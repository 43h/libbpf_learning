
all:
	make clean
	clang -O2 -g -target bpf -c sk_assign_k.c -o sk_assign_k.o
	clang sk_assign_u.c -lelf -lbpf -o main

clean:
	rm -rf *.o
	rm -rf main

install:
	tc qdisc add dev ens34 clsact
	tc filter add dev ens34 ingress bpf da obj sk_assign_k.o sec tc
	tc qdisc add dev ens35 clsact
	tc filter add dev ens35 ingress bpf da obj sk_assign_k.o sec tc

uninstall:
	tc filter del dev ens34 ingress
	tc qdisc del dev ens34 clsact
	tc filter del dev ens35 ingress
	tc qdisc del dev ens35 clsact

show:
	tc -s filter show dev ens34

route:
	iptables -t mangle -F
	iptables -t mangle -A PREROUTING -p tcp -d 192.168.160.5  --dport 80 -j TPROXY --tproxy-mark 0x1/0x1 --on-port 80 --on-ip 192.168.160.5
	#iptables -t mangle -A PREROUTING -p tcp -s 192.168.160.5 --sport 80 -j MARK --set-mark 1
	ip rule add fwmark 1 lookup 100
	ip route add local 0.0.0.0/0 dev lo table 100
	#ip route add local 192.168.160.5/32 dev ens35 table 100
	#ip route add local 192.168.160.5/32 dev lo table 100
	#ip route add local 192.168.160.5/32 dev ens34

route-clean:
	iptables -t mangle -F
	ip rule del fwmark 1 lookup 100
	ip route del local 0.0.0.0/0 dev lo table 100
	#ip route del local 0.0.0.0/0 dev ens35 table 100
	#ip rule add from 10.0.0.10 table John
	ip rule add fwmark 1 lookup 100
	ip rule add to 192.168.160.5/32  iif ens34 dport 80 table 100
	ip rule add from 192.168.160.5/32  iif ens35 sport 80 table 101
	ip route add local 192.168.160.5/32 dev ens34 table 100
	ip route add local 192.168.160.5/32 dev ens35 table 101
	iptables -t mangle -A PREROUTING -i ens34 -p tcp --dport 80 -j MARK --set-mark 1
	ip rule add fwmark 1 lookup 100
	ip route add local 192.168.160.5/32 dev lo table 100

link-up:
	ip link set ens34 up
	ip link set ens35 up
	ip link set ens34 promisc on
	ip link set ens35 promisc on
	ip addr add 192.168.140.3/24 dev ens34
	ip addr add 192.168.160.3/24 dev ens35

link-down:
	ip link set ens34 down
	ip link set ens35 down
	ip addr del 192.168.140.3/24 dev ens34
	ip addr del 192.168.160.3/24 dev ens35

br-up:
	ip link set ens34 up
	ip link set ens35 up
	ip link set ens34 promisc on
	ip link set ens35 promisc on
	brctl addbr br0
	brctl addif br0 ens33
	brctl addif br0 ens34
	ip link set br0 up
	ip addr add 192.168.140.3/24 dev br0
	ip addr add 192.168.160.3/24 dev br0

br-down:
	ip link set br0 down
	brctl delbr br0

br-rule:
	ebtables -t broute -F
	ebtables -t broute -A BROUTING -p ipv4 --ip-proto tcp --ip-dst 192.168.160.5 --ip-dport 80 -j redirect --redirect-target DROP
	ebtables -t broute -A BROUTING -p ipv4 --ip-proto tcp --ip-src 192.168.160.5 --ip-sport 80 -j redirect --redirect-target DROP

clean-br-rule:
	ebtables -t broute -F