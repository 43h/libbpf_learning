# xdp
---

## 简介
xdp（eXpress Data Path）是一种高性能的数据包处理技术，  
它运行在Linux内核中，可以在网络协议栈的最早阶段进行数据包处理。  
xdp可以用于实现各种网络功能，如防火墙、负载均衡、流量监控等。  
它通过在网络设备的驱动程序中插入eBPF（extended Berkeley Packet Filter）程序来实现数据包处理，具有低延迟、高吞吐量和灵活性的优势。