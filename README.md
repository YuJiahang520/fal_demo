FAL (Flash Abstraction Layer) Flash 抽象层，是对 Flash 及基于 Flash 的分区进行管理、操作的抽象层，对上层统一了 Flash 及 分区操作的 API (框架图如下所示)，并具有以下特性：

- 支持静态可配置的分区表，并可关联多个 Flash 设备；
- 分区表支持 **自动装载** 。避免在多固件项目，分区表被多次定义的问题；
- 代码精简，对操作系统 **无依赖** ，可运行于裸机平台，比如对资源有一定要求的 Bootloader；
- 统一的操作接口。保证了文件系统、OTA、NVM（例如：[FlashDB](https://gitee.com/Armink/FlashDB)） 等对 Flash 有一定依赖的组件，底层 Flash 驱动的可重用性；	
- 自带基于 Finsh/MSH 的测试命令，可以通过 Shell 按字节寻址的方式操作（读写擦） Flash 或分区，方便开发者进行调试、测试；

![](https://cdn.nlark.com/yuque/0/2025/png/2725789/1767149967565-d7734cd4-f483-4be8-9559-0970e79a78a7.png)



为了方便理解，**<font style="color:#74B602;">本文使用PC端的文件操作模拟Flash</font>**，移植后将展示如下操作：

+ 同时挂载两个Flash，可使用统一API操作
+ 一个Flash下挂载多个分区
+ 尝试对分区进行越界访问，查看报错
+ 使用统一API接口，实现记录程序运行次数



运行日志如下：

```c
[Driver1] Opened existing virtual flash file.
[Driver2] Opened existing virtual flash file.
[I/FAL] Flash Abstraction Layer (V0.5.99) initialize success.
FAL init success!
[I/FAL] ==================== FAL partition table ====================
[I/FAL] | name       | flash_dev     |   offset   |    length  |
[I/FAL] -------------------------------------------------------------
[I/FAL] | bootloader | pc_nor_flash1 | 0x00000000 | 0x00010000 |
[I/FAL] | param      | pc_nor_flash1 | 0x00010000 | 0x00010000 |
[I/FAL] | app        | pc_nor_flash1 | 0x00020000 | 0x00080000 |
[I/FAL] | data       | pc_nor_flash1 | 0x000a0000 | 0x00040000 |
[I/FAL] | falsh2     | pc_nor_flash2 | 0x00000000 | 0x00100000 |
[I/FAL] =============================================================
Find partition 'param' success! Addr: 0x00010000, Len: 65536
Find partition 'falsh2' success! Addr: 0x00000000, Len: 1048576
[Driver1] Read offset: 65536, size: 4
[Driver2] Read offset: 0, size: 4
count1 = 6, count2 = 6
[Driver1] Erased offset: 65536, size: 4096
[Driver2] Erased offset: 0, size: 4096
[Driver1] Write offset: 65536, size: 4
[Driver2] Write offset: 0, size: 4
[E/FAL] (fal_partition_read:414) Partition read error! Partition address out of bound.
ret1 = -1
[Driver1] Read offset: 130048, size: 4
ret2 = 4
[E/FAL] (fal_partition_read:414) Partition read error! Partition address out of bound.
ret3 = -1
```

