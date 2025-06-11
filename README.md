# eBPF 知识库
---
更新于 2024-09-13


## 简介
eBPF是一项革命性的技术，起源于Linux内核，可以在操作系统的内核中运行沙盒程序。  
它被用来安全和有效地扩展内核的功能，而不需要改变内核的源代码或加载内核模块。  
eBPF通过允许在操作系统内运行沙盒程序，应用程序开发人员可以在运行时，可编程地向操作系统动态添加额外的功能。 
然后，操作系统保证安全和执行效率，就像在即时编译（JIT）编译器和验证引擎的帮助下进行本地编译一样。  
eBPF 程序在内核版本之间是可移植的，并且可以自动更新，从而避免了工作负载中断和节点重启。

![kernel-arch](image/kernel-arch.png)  
![new-os](image/new-os-model.png)  

## 疑问
1. 如何开发一个最简单的 eBPF 程序？
2. 如何用 eBPF 追踪一个内核功能或函数？有很多种方法，举出对应的代码；
3. 有哪些方案能通过用户态和内核态通信？如何从用户态向内核态传送信息？如何从内核态向用户态传递信息？举出代码示例；
* Maps：eBPF Maps 是 eBPF 程序中最常用的一种数据结构，它们可以在用户态和内核态之间共享数据。用户态程序可以通过 BPF 系统调用来创建、更新、查找和删除 Map 中的元素，而内核态的 eBPF 程序可以在运行时访问这些元素。

* Perf Events：Perf Events 是一种可以将内核态的 eBPF 程序产生的数据发送到用户态的机制。eBPF 程序可以使用 bpf_perf_event_output 助手函数将数据发送到 Perf Event Buffer，然后用户态程序可以读取这个 Buffer 来获取数据。

* BPF 系统调用：用户态程序可以使用 BPF 系统调用来加载和管理 eBPF 程序和 Map。例如，bpf() 系统调用可以用来创建和加载 eBPF 程序，bpf_map_update_elem() 可以用来更新 Map 中的元素。

4. 编写一个你自己的 eBPF 程序，实现一个功能；
5. eBPF 程序的整个生命周期里面，分别在用户态和内核态做了哪些事情？

## 开发框架
- BCC
- BPFtrace
- libbpf
- libbpf-bootstrap
- Go eBPF library
- eunomia-bpf


## libbpf
llvm于2015年推出了可以将由高级语言编写的代码编译为eBPF字节码的功能，  
同时，eBPF 社区将 `bpf()` 等原始的系统调用进行了初步地封装，给出了`libbpf`库。  
这些库会包含将字节码加载到内核中的函数以及一些其他的关键函数。  
在Linux的源码包的`samples/bpf/`目录下，有大量Linux提供的基于`libbpf`的eBPF样例代码。

一个典型的基于 `libbpf` 的eBPF程序具有`*_kern.c`和`*_user.c`两个文件，
`*_kern.c`中书写在内核中的挂载点以及处理函数，  
`*_user.c`中书写用户态代码，完成内核态代码注入以及与用户交互的各种任务。



## eBPF可使用函数类型
- xdp：网络数据处理函数，拦截和处理网络数据包。
- tc:
- kprobe：插探函数，在指定的内核函数前或后执行。
- tracepoint：跟踪点函数，在指定的内核跟踪点处执行。
- raw_tracepoint：原始跟踪点函数，在指定的内核原始跟踪点处执行。
- perf_event：性能事件函数，用于处理内核性能事件。
- kretprobe：函数返回插探函数，在指定的内核函数返回时执行。
- tracepoint_return：跟踪点函数返回，在指定的内核跟踪点返回时执行。
- raw_tracepoint_return：原始跟踪点函数返回，在指定的内核原始跟踪
- fentry/fexit
- network devices (tc/xdp)
- network routes
- TCP congestion algorithms
- sockets (data level)

userspace functions (uprobes)
system calls

3. **Socket Filter 程序**:
   ```c
   SEC("socket")
   ```


9. **cgroup/skb 程序**:
   ```c
   SEC("cgroup/skb")
   ```

10. **cgroup/sock 程序**:
    ```c
    SEC("cgroup/sock")
    ```

11. **cgroup/sock_addr 程序**:
    ```c
    SEC("cgroup/sock_addr")
    ```

12. **cgroup/dev 程序**:
    ```c
    SEC("cgroup/dev")
    ```

13. **cgroup/bind4 程序**:
    ```c
    SEC("cgroup/bind4")
    ```

14. **cgroup/bind6 程序**:
    ```c
    SEC("cgroup/bind6")
    ```

15. **cgroup/connect4 程序**:
    ```c
    SEC("cgroup/connect4")
    ```

16. **cgroup/connect6 程序**:
    ```c
    SEC("cgroup/connect6")
    ```

17. **cgroup/post_bind4 程序**:
    ```c
    SEC("cgroup/post_bind4")
    ```

18. **cgroup/post_bind6 程序**:
    ```c
    SEC("cgroup/post_bind6")
    ```

19. **cgroup/sendmsg4 程序**:
    ```c
    SEC("cgroup/sendmsg4")
    ```

20. **cgroup/sendmsg6 程序**:
    ```c
    SEC("cgroup/sendmsg6")
    ```

这些段名帮助内核识别和加载不同类型的eBPF程序。根据具体需求选择合适的段名来编写eBPF程序。

## libbpf学习示例
* 1. libbpf环境安装
* 2. xdp, 打印tcp端口
* 3. tc, 打印IP信息
* 4. tracepoint

## Todo
---
* bpf_xdp_sk_lookup_tcp
* bpf_xdp_skc_lookup_tcp
* bpf_skc_lookup_tcp
* bpf_fib_lookup
* bpf_skb_load_bytes
* bpf_sk_lookup
* bpf_sk_assign

BPF_PROG_TYPE_SOCK_OPS

bpf_redirect_map

### xdp-->lvs
SEC("tp_btf/inet_sock_set_state")：关注 tcp 状态转变
SEC("kprobe/inet_release")：内核释放 socket 的时候会调用这个函数

ret = bpf_redirect(TARGET_INTF, BPF_F_INGRESS);

bpf_redirect_neigh()
bpf_redirect_peer()
bpf_spin_lock 

sockmap
bpf-sock-ops

echo_dispatch

BPF_PROG_TYPE_SK_LOOKUP

![alt text](image.png)


## 参考
https://blog.cloudflare.com/its-crowded-in-here  
http://vger.kernel.org/lpc_net2018_talks/dsa-xdp-kernel-tables-paper.pdf  