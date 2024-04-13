# TC
---

## tc 概述
"tc"是"Traffic Control"的缩写，  
它是Linux内核中的一个子系统，用于控制网络流量。
tc提供了一套工具和API，可以用于配置和管理网络设备的队列，以及对网络流量进行分类、调度和整形。

在Linux网络协议栈中，tc位于链路层，  
其主要工作是处理和控制数据包的发送和接收。  
tc使用了队列（Queueing discipline，简称 qdisc）结构来临时保存并组织数据包。  
在tc子系统中，qdisc提供了数据包入队和出队的接口，并在内部实现了排队算法。

tc还提供了filter和class机制，可以用于实现复杂的树形结构。  
filter被挂载到qdisc或class上，用于实现具体的过滤逻辑，而class则用于组织和分类数据包。

此外，tc还提供了classifier-action机制，  
可以在数据包匹配到特定filter时执行该filter所挂载的action对数据包进行处理，实现了完整的数据包分类和处理机制。



## man
- man tc
- man tc-bpf
  


## 命令
```bash
# 最开始的状态
tc qdisc show dev ens33
qdisc noqueue 0: root refcnt 2

# 创建clsact
tc qdisc add dev ens33 clsact
# 再次查看，观察有什么不同
tc qdisc show dev ens33

qdisc noqueue 0: root refcnt 2
qdisc clsact ffff: parent ffff:fff1
    
# 加载TC BPF程序到容器的veth网卡上
tc filter add dev ens33 egress bpf da obj tc-xdp-drop-tcp.o sec tc

# 再次查看，观察有什么不同
> tc qdisc show dev ens33
    qdisc noqueue 0: root refcnt 2
    qdisc clsact ffff: parent ffff:fff1
    > tc filter show dev ens33 egress
    filter protocol all pref 49152 bpf chain 0
    filter protocol all pref 49152 bpf chain 0 handle 0x1 tc-xdp-drop-tcp.o:[tc] direct-action not_in_hw id 24 tag 9c60324798bac8be jited
```