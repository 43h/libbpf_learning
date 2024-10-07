// SPDX-License-Identifier: GPL-2.0
// Copyright (c) 2019 Cloudflare Ltd.
// Copyright (c) 2020 Isovalent, Inc.

#include <stddef.h>
#include <stdbool.h>
#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/pkt_cls.h>
#include <linux/tcp.h>
#include <linux/if_packet.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

// struct 
// {
// 	__uint(type, BPF_MAP_TYPE_SOCKMAP);
// 	__uint(max_entries, 1);
// 	__type(key, __u32);
// 	__type(value, __u64);
// } server_map SEC(".maps");

SEC("tc")
int assign_test(struct __sk_buff *skb)
{
	//struct bpf_sock_tuple *tuple;

	void *data_end = (void *)(long)skb->data_end;
	void *data = (void *)(long)skb->data;

	//check ethernet header
	struct ethhdr *eth = (struct ethhdr *)(data);
	if ((void*)(eth + 1) > data_end) {
		return TC_ACT_OK;
	}
	//check ip protocol
	if (eth->h_proto != bpf_htons(ETH_P_IP))
	{
		return TC_ACT_OK;
	}

	struct iphdr *iph = (struct iphdr *)(eth + 1);
	if((void*)(iph + 1) > data_end) {
		return TC_ACT_OK;
	}

	if (iph->protocol != IPPROTO_TCP) {
		return TC_ACT_OK;
	}

	struct tcphdr *tcph = (struct tcphdr *)(iph + 1);
	if ((void*)(tcph + 1) > data_end) {
		return TC_ACT_OK;
	}

	if (tcph->dest == bpf_htons(80) || tcph->source == bpf_htons(80)) {
		bpf_printk("sport %d ---> dport: %d\n", bpf_ntohs(tcph->source), bpf_ntohs(tcph->dest));
		skb->mark = 1;
		bpf_skb_change_type(skb, PACKET_HOST);
	} else {
		return TC_ACT_OK;
	}

	struct bpf_sock_tuple * tuple = (struct bpf_sock_tuple *)&iph->saddr;
	if ((void *)(tuple + 1) > data_end) {
		return TC_ACT_OK;
	}
	unsigned int tuple_len = sizeof(tuple->ipv4);
	if ((void *)tuple + tuple_len > (void *)(long)skb->data_end)
		return TC_ACT_SHOT;
    bpf_printk("mark: %d\n", skb->mark);
	struct bpf_sock *sk = bpf_skc_lookup_tcp(skb, tuple, tuple_len, BPF_F_CURRENT_NETNS, 0);
	if (sk) {
		bpf_printk("sk->state: %d\n", sk->state);
		if (sk->state != BPF_TCP_LISTEN) {
	    	int ret = bpf_sk_assign(skb, sk, 0);
			bpf_printk("ret: %d\n", ret);
		}
		bpf_sk_release(sk);
	} else {
		bpf_printk("sk is null\n");
	}

	return TC_ACT_OK;
}

char _license[] SEC("license") = "GPL";