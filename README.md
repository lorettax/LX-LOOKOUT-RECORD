## 瞭望录协议
### LX-LOOKOUT-RECORD是关于我和我女朋友大婷宝感情故事孵化出的代码艺术产物，用以展望未来。

 这是一份有信仰的感情，等同于生命般的高度。每一句问号，每一句分号都代表了曾经感情生活的一篇故事，有趣又好玩。俗话说，任何想要从恋爱走到结婚的爱情，在最初至少都会有10次想要分手的想法、50次想要掐S 对方的冲动、100次的你争我闹和1000次质疑当初的选择，所以好的爱情都是磨合出来的，在这个世界上根本就没有什么天作之合 一帆风顺的爱情，所有的美满幸福都是经过鲜血淋漓的磨合和心如针扎的纠结换来的，所幸，我挺过来了。她说UDP已成为过去，TCP三次握手建立可靠连接，永不挥手，永远挥手失败。那么我将亲手来建立LX握手协议，来记录这些有趣的事。



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
