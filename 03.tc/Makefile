
bpf1:
	clang -O2 -g -target bpf -c tc_k.c -o tc_k.o
	clang tc_u.c -lelf -lbpf -o tc

clean:
	rm -rf *.o
	rm -rf tc

install:
	tc qdisc add dev ens33 clsact
	tc filter add dev ens33 ingress bpf da obj tc_k.o sec tc

uninstall:
	tc filter del dev ens33 ingress
	tc qdisc del dev ens33 clsact