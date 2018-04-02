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

完整的 SMQ 的构建指令为：

`build.sh build [--x32|--x64] [--bdynamic|--bstatic] [--debug] [--jni] `

### 环境要求

- Windows

Windows 下编译需要 Visual Studio 2010，如果编译 64 位版本还需要安装 64 位编译器。

- Linux

Linux 下编译需要`gcc xxx`版本及以上，需要`pthread`

### 编译选项

SMQ 编译完成之后，生成的目标文件命名方式为：

```
libsmq+<调试版标记>+<OS架构>.<输出库类型>
```

#### 动态库、静态库

#### 32位、64位

#### 调试版

#### 直接JNI绑定

## 快速开始

## 用户手册

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