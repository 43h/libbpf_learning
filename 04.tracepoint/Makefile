main:
	clang -O2 -g -target bpf -c bpf_k.c -o bpf_k.o
	bpftool gen skeleton bpf_k.o > bpf.skel.h
	clang bpf_u.c -lelf -lbpf -o main

clean:
	rm -rf *.o
	rm -rf bpf.skel.h
	rm -rf main