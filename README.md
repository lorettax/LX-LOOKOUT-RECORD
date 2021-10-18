## 瞭望录协议
### LX-LOOKOUT-RECORD是关于我和我女朋友大婷宝感情故事孵化出的代码艺术产物，用以展望未来。

 这是一份有信仰的感情，等同于生命般的高度。每一句问号，每一句分号都代表了曾经感情生活的一篇故事，有趣又好玩。俗话说，任何想要从恋爱走到结婚的爱情，在最初至少都会有10次想要分手的想法、50次想要掐S 对方的冲动、100次的你争我闹和1000次质疑当初的选择，所以好的爱情都是磨合出来的，在这个世界上根本就没有什么天作之合 一帆风顺的爱情，所有的美满幸福都是经过鲜血淋漓的磨合和心如针扎的纠结换来的，所幸，我挺过来了。她说UDP已成为过去，TCP三次握手建立可靠连接，永不挥手，永远挥手失败。那么我将亲手来建立LX握手协议，来记录这些有趣的事。

### 协议背景故事

秃头小妹妹？求佳人求富贵？好喜欢你呀？项目经历：前男友的故事？关于爱情的交流；向灰色的代码世界注入一丝温暖的色彩；爱情法则：及时复盘；我错了，我不该瞒着你；一声谢谢！我要爱上你了；寻爱？心痛的开始；将你从世界中找出来；我们去河边玩吧，采花？爱的承诺：我爱你，这辈子只爱你；还未离开时的责怪；选择：错了也要走的路，撞破南墙也不回头；美好的日常时光，我们是好朋友；我们是铁铁的哥们；神奇的安河桥北？我不喜欢你了；我太烦了，你又要离开了吗？徘徊的十字路口？河边坐过的石头 ，渭河之上的船；你喜欢上了别人？向日葵海洋；威胁,好痛？我们痛吗，你恨我吗？登门拜访；决裂：单曲循环《可惜不是你》？雨夜失魂般奔跑，夜班火车； 马路边的晕厥，医生姐姐的劝解；走你走过的路；一次次打碎，一次次黏在一起的心；后悔一小步，绝望的转身；辞职，颓废人生，想一直跟着你；转身？抛弃？孤独？警察来了； 托外卖小哥送去的档案袋；晚安：一束狗尾草陪我闯世界；逍遥乐自在，人生新生活真香；瞭望录：是你回来了吗？我好痛，为什么？展望未来，回顾以后；我喜欢你；脱单文案：TCP与UDP的故事？百味人生：看山看海看世界；赤城回应 ：报团取暖；满怀期待的过年；我不喜欢你，我们不合适？求变；再次出现的...；一起喝乌苏么？信任标尺；终于在一起了，修成了半个正果，努力挣钱养家糊口，守护你；愿余生故事美好又有趣.....

![1b5eef07c3b1334c69e4ca19d6e2906](https://user-images.githubusercontent.com/50191422/137711862-182b3e1a-0fcc-44c6-87fd-4cafce2d5e0a.jpg)

![65e7524f034ecc819c9a7087a4d618c](https://user-images.githubusercontent.com/50191422/137711934-47640f4d-249f-406a-a5bb-a043524a2b04.jpg)
过了一年多的狗尾巴草，哈哈哈~
![8be52fba84db844572f9a86331bd0f1](https://user-images.githubusercontent.com/50191422/137711966-1a8e419d-04ca-4f66-8ec9-a0fa11afe8d2.jpg)


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
