# ipv4-stack-lab
# C教学作业项目

A minimal educational IPv4 stack implementation in C.  
学生级别的自我练习项目。本项目可能潜在诸多错误，仅用于自我练习  
This project is an educational / student-level IPv4 stack implementation.  
It is NOT production-ready and contains known limitations and simplifications.

测试流程中使用了 AI 工具作为辅助，核心 IP 协议栈逻辑由作者实现。

AI tools were used as an aid during testing, while the core IP stack logic was implemented by the author.

这是我第一次将该项目上传至 GitHub。

This is my first time uploading this project to GitHub.

---

【主要说明】  
本项目实现了IPv4协议中最基础的内容，涵盖基本检验、收发数据报、呈递上下层，及测试所必需的虚拟NIC；  
测试输入、输出为二进制文件(.bin)，设计了方便改造的上下层接口。

---

【实现缺陷】  
1.本项目未实现基本IP，ICMP Echo和部分错误汇报以外的一切功能  
2.本项目未实现并发  
3.本项目不支持：转发、路由表、ARP与衔接物理网卡；不支持IP选择头部等  
4.本项目可能潜在诸多错误，这些错误由于作者能力，尚未甄别

---

【如何测试】  
1.修改test_env.c中的信息，包括MAC/IP/子网掩码  
2.准备好一系列.bin文件，如果测试收信，确保所有文件都是IPv4数据报；如果是发信，确保都是恰当的第四层数据  
3.在编译的可执行文件下，新建tests文件夹用于输入，新建targets文件夹用于输出  
4.调整main.c的内容，选择目的IP，及测试项目  
5.运行，检查输入输出是否正确

---

【结构详解】

```text
.
├── include/   
│
├── src/
│   ├── checksum.c          # 校验和计算
│   ├── icmp.c              # ICMP 处理
│   ├── ipv4_general.c      # IPv4 通用工具
│   ├── ipv4_handlers.c     # IPv4 上层协议分发 / 注册
│   ├── ipv4_input.c        # IPv4 输入处理
│   ├── ipv4_output.c       # IPv4 输出 / 封装
│   ├── netif.c             # fake / mock 网络接口
│   └── netstack_init.c     # 协议栈初始化
│
└── test/                   # 测试所需函数及main函数

```
---

【许可证】

本项目采取MIT，因为价值不是很大，欢迎各位批评指正

ipv4.h中使用了部分MIT协议的代码，其中已经注明

Some of the ipv4.h code is adapted from an MIT-licensed implementation.
