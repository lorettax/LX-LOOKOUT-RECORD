## 瞭望录协议
### LX-LOOKOUT-RECORD仓库是实现基本的TCP/IP网络协议栈。




## 协议架构图：

![384eb4504bfb5a1325a5b36d50a1f88](https://user-images.githubusercontent.com/50191422/137712109-44750b88-507b-4e10-9531-0c8da4cf655a.png)


### 协议环境部署：

C++17，Centos7 or Ubuntu18.04，VirtualBox

编译器版本：gcc, g++ version 8.x

调试工具：GDB, cmake version 3，wireshark

依赖包：clang-tidy version  7，clang-format version  7，libpcap development headers (libpcap-dev on Debian-like distributions)，iptables，mininet 2.2.0 or later，tcpdump，telnet，wireshark，socat，netcat-openbsd，GNU coreutils，bash，doxygen，graphviz

### 构建：

```bash
$ mkdir -p <path to source> build
$ cd ./build
$ cmake ..
$ 并行构建：
$ make -j (num) (num:并行构建数, j:并行构建开关)
```

- 测试：

```bash
$ make check
# 性能测试：
$ ./loveapps/tcp_benchmark
```

### 协议效果：

```bash
 # Aliyun, Centos7.
 # 协议性能throughput最高可达1.9Gbit/s
```

### 状态机参考

![b8bc2981befa33da2e3d9e62de65407](https://user-images.githubusercontent.com/50191422/137712214-626768a5-5125-4223-89c3-c8d694c85812.png)


### 下一版改进

引入网络流攻击防护。

进一步建立可靠网络流，优化性能。

封装界面客户端。


