// #include "../vmlinux.h"
#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#include <linux/if_ether.h>
#include <linux/in.h>
#include <linux/tcp.h>
#include <linux/ip.h>

struct {
	__uint(type, BPF_MAP_TYPE_HASH);
	__uint(max_entries, 4096);
	__type(key, unsigned int);
	__type(value, unsigned long);
} my_map SEC(".maps");

SEC("xdp")
int xdp_pass(struct xdp_md *ctx)
{
    void *data = (void *)(long)ctx->data;           //报文起始地址
    void *data_end = (void *)(long)ctx->data_end;   //报文结束地址
    int pkt_sz = data_end - data;                   //报文实际长度

    int tcp_off = sizeof(struct ethhdr) + sizeof(struct iphdr) + sizeof(struct tcphdr);  //TCP头部偏移

    if (data + tcp_off > data_end)  //检测报文长度
    {
        return XDP_PASS;
    }

    struct ethhdr *eth = data;
    if (eth->h_proto != bpf_htons(ETH_P_IP))  //非IP直接通过
    {
        return XDP_PASS;
    }

    struct iphdr *ip = data + sizeof(struct ethhdr); //非TCP直接通过
    if (ip->protocol != IPPROTO_TCP)
    {
        return XDP_PASS;
    }

    struct tcphdr *tcp = data + sizeof(struct ethhdr) + sizeof(struct iphdr); //获取tcp头部
    static char log[] = "tcp port: %u-->%u\n";
    bpf_trace_printk(log, sizeof(log), bpf_ntohs(tcp->source), bpf_ntohs(tcp->dest)); //打印源和目的端口
    
    if(tcp->source == bpf_htons(22)) //Todo: 过滤ssh流量
    {
        unsigned int key = 0;
        unsigned long time_ns = bpf_ktime_get_ns();
        bpf_map_update_elem(&my_map, &key, &time_ns, BPF_ANY);
    }
    return XDP_PASS;
}

char _license[] SEC("license") = "GPL";