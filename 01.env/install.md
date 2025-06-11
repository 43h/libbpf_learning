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



## vmlinux.h
该文件是根据当前Linux内核的调试信息（BTF，BPF Type Format）自动生成的头文件，  
里面包含了内核中各种结构体、枚举、类型定义。  
例如：task_struct、file、sock 等等。  
这样，eBPF 程序就可以直接访问和操作内核数据结构，确保eBPF程序和当前运行的内核结构体保持一致。