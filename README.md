# 功能目标

### 项目目标
学习网络编程
1. 实现网络报文发送/接收
2. 实现报文的过滤存储

### TODO
1. 网络编程
2. 数据包转发

### 项目计划
1. 服务 ARP 
2. 公共库 - euv库
3. 公共库 - 日志
4. 服务 IPv4 & ICMP
5. 实现服务管理

### 项目构建
- apps 应用程序
  - network 数据包转发
  - socket 网络编程
  - tips epoll 源码阅读
  - uev 事件魔心
  - daemon 守护进程
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
  - lib_xml xml读取
    - protobuf_c proto - bin 转换

### Make


问题 1：

在`gitignore`文件中添加下列忽略所有的 `.DS_Store` 文件

```
# Ignore all .DS_Store files recursively
**/.DS_Store
```

如果你的 `.gitignore` 文件已经存在，只需在文件中添加上述规则即可。如果 `.DS_Store` 文件已经被跟踪并提交到版本控制系统，你可能还需要从版本历史中删除它们。你可以使用以下命令：


```
git rm --cached '**/.DS_Store'
git commit -m "Remove all .DS_Store files from version control"
```


如何在编译程序之前使用检查查看需要安装的库？

### 参考链接
1. https://github.com/saminiir/level-ip/tree/e9ceb08f01a5499b85f03e2d615309c655b97e8f