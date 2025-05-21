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