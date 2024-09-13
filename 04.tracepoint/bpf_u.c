#include <linux/types.h>
#include <sys/resource.h>
//#include <bpf/bpf_helpers.h>
#include "bpf.skel.h"

int main() {
  struct bpf_k *skel = bpf_k__open();
  bpf_k__load(skel);
  bpf_k__attach(skel);
  // 打印内核调试文件的内容
  system("cat /sys/kernel/debug/tracing/trace_pipe");
  return 0;
}