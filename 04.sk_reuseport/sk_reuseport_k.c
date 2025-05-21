#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

struct {
  __uint(type, BPF_MAP_TYPE_REUSEPORT_SOCKARRAY);
  __uint(max_entries, 8);
  __type(key, __u32);
  __type(value, __u64);
} reuseport_array SEC(".maps");

SEC("sk_reuseport")
int select_socket(struct sk_reuseport_md *ctx)
{
    const __u32 num_sockets = 4;
    __u32 hash = ctx->hash;

    hash ^= hash >> 16;
    __u32 index = hash % num_sockets;

    bpf_printk("sk_reuseport: hash=0x%x, selected index=%u\n", hash, index);

    if(bpf_sk_select_reuseport(ctx, &reuseport_array, &index, 0) < 0) {
        bpf_printk("bpf_sk_select_reuseport failed\n");
    } else{
		bpf_printk("bpf_sk_select_reuseport succeeded\n");
	}

    return SK_PASS;
}

char LICENSE[] SEC("license") = "GPL";
