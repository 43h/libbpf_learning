/**
 * @file bpf_u.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-09-11
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdlib.h>

int main()
{
  system("cat /sys/kernel/debug/tracing/trace_pipe"); // 打印内核调试文件的内容
  return 0;
}