#include <linux/types.h>
#include <sys/resource.h>
// #include <bpf/bpf_helpers.h>
#include "bpf.skel.h"

int main()
{
  struct sk_assign_k *skel = sk_assign_k__open();
  if (skel == NULL)
  {
    return 0;
  }

  sk_assign_k__load(skel);
  sk_assign_k__attach(skel);
  // 打印内核调试文件的内容
  system("cat /sys/kernel/debug/tracing/trace_pipe");
  return 0;
}