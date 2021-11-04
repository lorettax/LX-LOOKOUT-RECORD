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

进一步建立可靠网络流，挥手永远失败。

封装界面客户端，以循环会话模式轮转瞭望录。

### 背景附图：

![1b5eef07c3b1334c69e4ca19d6e2906](https://user-images.githubusercontent.com/50191422/137711862-182b3e1a-0fcc-44c6-87fd-4cafce2d5e0a.jpg)

![65e7524f034ecc819c9a7087a4d618c](https://user-images.githubusercontent.com/50191422/137711934-47640f4d-249f-406a-a5bb-a043524a2b04.jpg)
过了一年多的狗尾巴草，哈哈哈~
![8be52fba84db844572f9a86331bd0f1](https://user-images.githubusercontent.com/50191422/137711966-1a8e419d-04ca-4f66-8ec9-a0fa11afe8d2.jpg)
