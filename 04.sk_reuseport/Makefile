# Makefile for eBPF project

.PHONY: all 

all: clean 
	clang -g -O2 -Wall -target bpf -c sk_reuseport_k.c -o sk_bpf.o
	gcc -g -Wall sk_reuseport_u.c -o main -lbpf -lelf

clean:
	rm -f *.o
	rm -f main