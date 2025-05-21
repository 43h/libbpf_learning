#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8080
#define SOCK_NUM 4

int main() {
    // 1. load BPF program
    struct bpf_object *obj = bpf_object__open_file("sk_bpf.o", NULL);
    if (!obj || libbpf_get_error(obj)) {
        fprintf(stderr, "Failed to open BPF object\n");
        goto end;
    }

    if (bpf_object__load(obj)) {
        fprintf(stderr, "Failed to load BPF object\n");
        goto end;
    }

	// 2. find reuseport_array
    struct bpf_map *reuseport_array = bpf_object__find_map_by_name(obj, "reuseport_array");
    if (!reuseport_array) {
        fprintf(stderr, "Failed to find map 'sock_hash'\n");
        bpf_object__close(obj);
        return 1;
    }

    int map_fd = bpf_map__fd(reuseport_array);
    if (map_fd < 0) {
		fprintf(stderr, "Failed to get map FD\n");
		goto end;
	}

	// 3. attach BPF program to socket
    struct bpf_program *prog = bpf_object__find_program_by_name(obj, "select_socket");
    if (!prog) {
        fprintf(stderr, "Failed to find BPF program with title 'sk_reuseport/select_socket'\n");
        goto end;
    }

    int prog_fd = bpf_program__fd(prog);
    if (prog_fd < 0) {
        fprintf(stderr, "Failed to get program FD\n");
        goto end;
    }

	// 4. create/setops/bind/listen sockets
	struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
		.sin_addr.s_addr = inet_addr(IP),
    };
    int socks[SOCK_NUM];
    int opt = 1;

    for (int i = 0; i < SOCK_NUM; i++) {
        socks[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (socks[i] < 0) {
            perror("socket");
            goto end;
        }

        if (setsockopt(socks[i], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            perror("setsockopt(SO_REUSEADDR)");
            goto end;
        }
        if (setsockopt(socks[i], SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt)) < 0) {
            perror("setsockopt(SO_REUSEPORT)");
            goto end;
        }

		// check SO_REUSEPOR flag
		int get_opt = 0;
		socklen_t get_optlen = sizeof(get_opt);
		if (getsockopt(socks[i], SOL_SOCKET, SO_REUSEPORT, &get_opt, &get_optlen) < 0) {
			perror("getsockopt(SO_REUSEPORT)");
			goto end;
		}
		if (get_opt != 1) {
			fprintf(stderr, "SO_REUSEPORT not set correctly on socket %d\n", i);
			goto end;
		}

		// !!! important attach BPF program to the first socket
		if (i == 0) {
            if (setsockopt(socks[i], SOL_SOCKET, SO_ATTACH_REUSEPORT_EBPF, &prog_fd, sizeof(prog_fd)) < 0) {
            perror("setsockopt(SO_ATTACH_REUSEPORT_EBPF)");
            goto end;
            }
	    }

        if (bind(socks[i], (struct sockaddr *)&addr, sizeof(addr)) < 0) {
            perror("bind");
            goto end;
        }

        if (listen(socks[i], 128) < 0) {
            perror("listen");
            goto end;
        } 
    }

	// 5. update map with socket fds
    for (int i = 0; i < SOCK_NUM; i++) {
		__u32  key = i; 
		__u64 value = (__u64)socks[i];
		printf("key=%u, fd=%llu\n", key, value);
		if(bpf_map_update_elem(map_fd, &key, &value, BPF_NOEXIST) < 0) {
			fprintf(stderr, "Failed to update map: %s\n", strerror(errno));
		} else{
			printf("Updated map with key=%u, fd=%llu\n", key, value);
		}
	}

	// 6. fork processes to handle connections
	for (int i = 0; i < SOCK_NUM; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork");
			goto end;
		} else if (pid == 0) {
			// 子进程：每个进程只处理自己的 socket
			printf("Process %d waiting on socket %d\n", getpid(), i);
			while (1) {
				int client = accept(socks[i], NULL, NULL);
				if (client < 0) {
					perror("accept");
					continue;
				}
				char msg[64];
				snprintf(msg, sizeof(msg), "Hello from process %d (socket %d)\n", getpid(), i);
				write(client, msg, sizeof(msg) - 1);
				close(client);
			}
			exit(0);
		}
	}

	for (int i = 0; i < SOCK_NUM; i++) {
		wait(NULL);
	}

    printf("Listening on port %d with %d sockets and attached BPF program.\n", PORT, SOCK_NUM);
    pause(); // 保持运行

end:
    for (int i = 0; i < SOCK_NUM; i++) {
        //!!! important, remove sockets from map before closing, skip
		if (socks[i] > 0)
            close(socks[i]);
    }
    if (obj)
        bpf_object__close(obj);
    return 0;
}
