all:
	clang -O2 -g -target bpf -c bpf_k.c -o bpf_k.o
	clang bpf_u.c -lelf -lbpf -o main

clean:
	rm -rf *.o
	rm -rf main

install:
	ip link set dev ens33 xdp obj bpf_k.o sec xdp

uninstall:
	ip link set dev ens33 xdp off