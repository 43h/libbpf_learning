
drop:
	clang -O2 -g -target bpf -c tc-xdp-drop-tcp.c -o tc-xdp-drop-tcp.o

stat:
	clang -O2 -g -target bpf -c tc-xdp-statistics.c -o tc-xdp-statistics.o

clean:
	rm -rf *.o

install:
#tc qdisc add dev ens33 clsact
#tc filter add dev ens33 ingress bpf da obj tc_k.o sec tc

uninstall:
#tc filter del dev ens33 ingress
#tc qdisc del dev ens33 clsact