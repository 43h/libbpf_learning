# libbpf环境配置
---
更新于 2024-09-22

## 测试环境
* 系统：Ubuntu 24.04.1 LTS
* 内核：6.8.0-60-generic

## 安装
```shell
sudo apt install clang
sudo apt install libbpf-dev
sudo apt install linux-headers-`uname -r`
sudo ln -s /usr/include/x86_64-linux-gnu/asm /usr/include/asm
bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
```



## 头文件
* vmlinux.h
该文件是根据当前Linux内核的调试信息（BTF，BPF Type Format）自动生成的头文件，    
里面包含了内核中各种结构体、枚举、类型定义。    
例如：task_struct、file、sock 等等。   
这样，eBPF 程序就可以直接访问和操作内核数据结构，确保eBPF程序和当前运行的内核结构体保持一致。

* linux/bpf.h
内核源码，通常位于 /usr/include/linux/bpf.h  
定义了eBPF程序和用户空间交互的核心结构体、常量、指令集、系统调用接口等（如 struct bpf_insn、BPF_MAP_TYPE_*、BPF_PROG_TYPE_* 等）  
用途：既可用于内核空间，也可用于用户空间（如通过 bpf() 系统调用加载 eBPF 程序时）。

* bpf/xxx.h
libbpf项目，通常安装在 /usr/include/bpf/目录下  
提供了eBPF用户空间辅助函数和宏，便于开发和调试。例如：
> <bpf/bpf_helpers.h>：
>> eBPF 程序内调用的辅助函数声明（如 bpf_map_lookup_elem、bpf_trace_printk 等）。

> <bpf/bpf_endian.h>：
>> 字节序转换相关的宏和函数。

> <bpf/libbpf.h>：
>>libbpf 用户空间 API。主要用于 eBPF 程序开发（内核侧和用户侧辅助），简化开发流程。