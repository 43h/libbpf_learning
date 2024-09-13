#include <stdlib.h>

int main()
{
    system("cat /sys/kernel/debug/tracing/trace_pipe"); // 打印内核调试文件的内容
    return 0;
}