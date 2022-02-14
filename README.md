# 使用cmake 构建的cpp简单库(log,sock,oop_sock)
 这是我本人意义上的第一个项目的第一个版本的发布，不断不断的调试代码，不断地重写
 ，不断地进行修改，终于在今天下午完成了最基本地一些要求。
 ## 日志模块
 日志模块是跨平台地，因为我少不了要在win写代码，用法也很简单，首先使用auto logger = SU_LOT_ROOT
 获得一个logger,然后使用SU_LOT_XXX(logger) << 进行流式地写入日志。具体可以看代码。该日志系统是模仿
 sylar的日志，同时做了一些我自己的调整，sylar在b站上,github上都有。
 ## sock模块
借助日志系统，区分了client和server的日志器名称，同时对正在发生都有相应日志输出，帮助
我们调试bug,目前还很简陋，因为我socket编程还没学完。
## oop_sock模块
借助上述两个系统，满足了我简化代码的需求，设计基本没有，但确实极大的减小了我的代码量。

## 后续
   sock学完会把sock和oop_sock进行完善，同时加入一些其他板块的东西，比如：线程和epoll，协程什么的
   不过还有好久，可以先star。预计一个月内上线sock部分。
   
 ### 如何运行
   chmod -R 777 install.sh | ./install.sh
