# 功能目标

### 项目目标
学习网络编程
1. 实现网络报文发送/接收
2. 实现报文的过滤存储

### 项目计划
1. 服务 ARP 
2. 公共库 - euv库
3. 公共库 - 日志
4. 服务 IPv4 & ICMP
5. 实现服务管理

### 项目构建
- apps 应用程序
- build 编译中间文件
- include 头文件
  - proto proto文件
    - msgproto  消息传递prto
    - confproto 配置文件proto
  - *.h 文件
- libs 库文件
  - lib_hash hash表
  - lib_list list链表s
  - lib_log 日志
  - lib_str 字符串
  - lib_telnet telnet调试功能
  - lib_uev 一个事件模型公共库

### 参考链接
1. https://github.com/saminiir/level-ip/tree/e9ceb08f01a5499b85f03e2d615309c655b97e8f