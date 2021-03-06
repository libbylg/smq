# SMQ

SMQ 是一个基于共享内存的高性能的同节点进程通信库。

```
   ______  _______ 
  / __/  |/  / __ \
 _\ \/ /|_/ / /_/ /
/___/_/  /_/\___\_\

```

## 特性清单

- 高性能
- 多平台支持
- 跨进程共享内存消息池
- 无锁队列
- 直接jni支持
- 嵌入式
- NIO友好
- 充分的测试
- 简单的接口


## 编译

### 傻瓜式编译

最简单的编译方式就是直接执行下面的指令：

```sh
./build.sh
```

该编译指令将自动根据编译环境或者脚本里内置的缺省值，完成整个编译过程。通常来说这个就足够了。

但是，如果您希望对 SMQ 的编译进行更精确地控制，可以通过指令`build.sh -h`来查看详细的编译参数说明。

### 完整的编译选项

完整的 SMQ 的构建指令为：

```sh
build.sh [--x32|--x64] [--bdynamic|--bstatic] [--debug] [--jni]
```

各选项含义如下：

* 32位/64位

	-	`--x32` 表示生成32位版本的 smq 库

	-	`--x64` 表示生成64位版本的 smq 库

如果不指定，编译脚本将自动根据操作系统类型来确定。

* 动态库/静态库

	-	`--bdynamic` 表示生成动态连接库

	-	`--bstatic`  表示生成静态连接库

如果不指定，默认为`--bdynamic`，即默认会生成动态库。

* 调试版/非调试版

	`--debug`	表示生成调试版本，如果不带`--debug`表示生成非调试版。调试版一般用于开发阶段，方便分析代码。默认为非调试版本。

* 支持 JNI 绑定

通过指定`--jni`参数，可以让编译脚本直接编译出支持 JNI 的动态库。这样，该动态库可以直接被 Java 调用。默认为不支持 JNI。

### 生成的目标文件

SMQ 生成的目标文件的命令方式为：

```text
libsmq[d][32|64][jni]-x.y.z.[so|lib|dylib|dll]
```

其中，`x.y.z`是动态库的版本号。

比如，`libsmqd64jni.so` 这个目标文件意味着----该库是 SMQ 的64位调试版且带jni支持的动态库。基本上来说，和我们编译 SMQ 的编译选项是一一对应的。


### 对编译环境要求

- Windows

Windows 下编译需要 Visual Studio 2010，如果编译 64 位版本还需要安装 64 位编译器。

- Linux

Linux 下编译需要`gcc xxx`版本及以上，需要`pthread`


### 生成 SMQ 发布包

执行下面的指令，将自动完成编译和生成发布包的完整流程。

```
build.sh package <OPTIONS>
```

### 运行 SMQ 测试用例

如果我们想在当前环境进行一次 SMQ 基本功能测试，可以使用下面的指令：

```
build.sh test ... 
```


## 快速开始

- 连接 SMQ

当我们需要将 SMQ 应用于我们自己的系统中时，首先我们需要将 SMQ 连接到我们的系统代码中一起编译。这些是基本的开发常识。比如，`-lsmq64`就是通常用到的连接指令。

- 引入头文件

SMQ 的所有接口都定义在 `smq.h`文件中，如果我们通过`build.sh package`来生成发布包，你可以在最终生成的发布包中找到该文件。

- 写一个服务端

- 写一个客户端

- 总结

## 参考手册

### SMQ 的基本概念

- 角色

- SMQ 实例

- SMQ 的配置

- SMQ 的消息

### 设置全局参数

### 初始化

### 创建和释放消息

### 发送和接收消息

### 错误处理

### 重定向日志

### Dump 共享内存

### 解析共享内存数据

## 高级

### 可移植性

#### 支持更多操作系统

#### 支持更多编译器

#### 嵌入式环境
malloc free重定义

### 如何与NIO配合

### 避免共享内存泄漏

### Java语言支持-JNI接口

### 性能

### 感谢

- [ASCII Generator](http://www.network-science.de/ascii/) : This is a very intreasting tool to create LOGO of ASCII.

