# libbpf环境配置
---
更新于 2024-04-12

## 测试环境
* Ubuntu 22.04

## 安装
```shell
sudo apt install clang
sudo apt install libbpf-dev
apt install linux-headers-`uname -r`
sudo ln -s /usr/include/x86_64-linux-gnu/asm /usr/include/asm
bpftool btf dump file /sys/kernel/btf/vmlinux format c > vmlinux.h
```