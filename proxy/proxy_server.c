#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>

#define PORT 8080

int main() {
	int sockfd;
	struct sockaddr_in server_addr;

	// 创建套接字
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket creation failed");
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		perror("setsockopt SO_REUSEADDR failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	// 设置套接字选项以启用透明代理
	int optval = 1;
	if (setsockopt(sockfd, SOL_IP, IP_TRANSPARENT, &optval, sizeof(optval)) < 0) {
		perror("setsockopt IP_TRANSPARENT failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	memset(&server_addr, 0, sizeof(server_addr));

	// 填写服务器地址信息
	server_addr.sin_family = AF_INET;
	if (inet_pton(AF_INET, "192.168.160.5", &server_addr.sin_addr) <= 0) {
		perror("inet_pton failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}
	server_addr.sin_port = htons(PORT);

	// 绑定套接字
	if (bind(sockfd, (const struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
		perror("bind failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	// 监听连接
	if (listen(sockfd, 5) < 0) {
		perror("listen failed");
		close(sockfd);
		exit(EXIT_FAILURE);
	}

	printf("Listening on port %d\n", PORT);

	while (1) {
		int new_socket;
		struct sockaddr_in client_addr;
		socklen_t client_len = sizeof(client_addr);

		// 接受连接
		if ((new_socket = accept(sockfd, (struct sockaddr *)&client_addr, &client_len)) < 0) {
			perror("accept failed");
			close(sockfd);
			exit(EXIT_FAILURE);
		}

		printf("Accepted connection from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

		char buffer[1024];
		int bytes_read;

		while ((bytes_read = read(new_socket, buffer, sizeof(buffer) - 1)) > 0) {
			buffer[bytes_read] = '\0'; // Null-terminate the buffer
			printf("Received: %s\n", buffer);

			send(new_socket, buffer, strlen(buffer), 0);
		}

		if (bytes_read < 0) {
			perror("read failed");
		}
		// 处理连接
		// ...

		close(new_socket);
	}

	close(sockfd);
	return 0;
}