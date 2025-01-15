# H7_ROBOT

为基于H723VGT6主控的达妙MC02开发板开发的一套人形机器人控制程序，主要功能为机器人手臂的控制

## 开发工具

==仓库根目录下的`.Doc`文件夹中有另外一些关于配置环境和开发更详尽的说明！==

介绍完整的工作流。

### 工具链

**使用CLion开发STM32的教程：https://xdgstudio.feishu.cn/docx/Doogdh75CoymNUxvkh2cWsHHnjg**

请大家参考此教程配置好环境，使用CLion开发STM32有利于养成良好的编程习惯。

（PS：很多高手使用CLion开发STM32，稚辉君曾分享过CLion开发STM32的教程，2023年电赛Ti杯开源的工程亦为CLion工程）

强烈推荐使用arm-gnu工具链进行编译（arm-none-eabi-xxx）。

官方下载地址：[Arm GNU Toolchain Downloads – Arm Developer](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads)

> 仍然支持使用arm-cc工具链（即keil默认的工具链）进行开发，在cubemx初始化时生成MDK项目即可，然后再手动添加basic_framework的所有头文件和源文件。但非常不建议这样做，arm-cc仅支持单线程编译且编译优化选项远不如arm-gnu多，自定义程度同样不比。~~若你一定要这样做，则可以在VSCode中安装keil assistant插件。~~

### IDE?

使用**VSCode**作为“IDE”，也支持Clion/Visual Studio等现代化IDE（需要自行配置）。需要的插件支持均已经在[VSCode+Ozone使用方法.md](.Doc/VSCode+Ozone使用方法.md)中给出。通过VSCode强大的插件系统、language server以及代码补全高亮助力效率倍增。编译则使用集成的task进行，还可以将开发环境终端加入VSCode进一步提升体验。基本的调试如变量&寄存器查看均已在插件中提供支持，`launch.json`可以进行高自由度的自定义。

`Git`集成与gitlens/gitgraph/githistory额外插件补充让版本管理和协作从未如此简单，`live share`把你的伙伴们聚在一起集思广益，一同对抗困难的bug。更多好用的插件、特性和开发技巧请参考"**如何使用本框架**"章节。

> **不论如何，请不要使用KEIL作为你的代码编辑器。**

### 调试和性能分析

- 基础的调试可以在VSCode中完成。cortex-debug插件的最新版本已经支持多个gdb-server（jlink/stlink/openocd/pyocd）的live watch（动态变量监视）和变量示波器（可视化）。若不是有特别的需求，*请勿使用串口调试器*。

- 有高速变量查看和数据记录、多路数据可视化的需求（如进行pid参数整定、查找难以定位的bug）时，使用**Segger Ozone**。
- FreeMaster也可以作为调试的备选项。
- 基本的、日常性能分析可以通过`bsp_dwt`完成。若要分析关于任务运行和每个函数执行的详细信息和时间，推荐使用**Segger Systemviewer**。

