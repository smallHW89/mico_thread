# mico_thread
a simple  coroutine 

一个简单的协程实现代码，just for fun
目前协程， 在互联网后台开放中，比较流行。
自身支持协程的语言有 go-lang ,er-lang  等。
C，C++  语言不支持协程， 有很多协程库：libgo, libtask 等。
很多公司也有自己内部的基于协程的框架，比如本人所在公司的很多团队都有用。
协程的原理，网络上很多博客文章都有介绍，这里不细说了。原理虽然明白了，但是只有真正实现个dameon才能算真正明白。


V0.0.1 mico_thread:
c/c++开发，基于Linux/x86-64/386/486平台， 用汇编实现 保存上下文，恢复上下文的接口。  汇编只考虑linux下的intel x86-64, x386这些 比较常见的 处理器架构。


