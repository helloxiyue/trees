## 什么是libcstl
**libcstl是使用标准C语言编写的通用的数据结构和常用算法的库。**
libcstl模仿STL的接口形式，包括序列容器，关联容器，容器适配器， 迭代器，函数和算法。libcstl为C编程中的数据管理提供了方便简易的程序库。 libcstl除了包括容器，迭代器，函数和算法四部分外，还包括类型机制以及内存管理。 类型机制允许用户在容器中保存任何类型的数据，这其中包括C内建的类型， libcstl内建的类型以及用户自定义的类型。内存管理采用内存池的方式帮助用户减少内存分配带来的碎片。 libcstl可以很好的运行在类Unix系统上，如Linux, FreeBSD等。 运行在Windows上时由于VC对C99的某些特性不支持，所以一些功能会受到限制。
源码下载地址：[http://libcstl.org/](http://libcstl.org/)，下载最新版本v2.3.0。

## libcstl在linux上的安装
libcstl在linux上的安装很简单，在linux上使用如下的命令就可以完成libcstl的安装：
```
\$ ./configure
\$ make
\# make install
```

## libcstl在windows和VC上的安装
官方发布说，libcstl-2.4（也就是下个版本）支持VC2010, VC2012, VC2013，但原定2015-05-18发布日期一拖再拖，所以想在win平台下使用libcstl的读者需要自己安装。在windows上的安装，主要是为了支持GnuWin gcc编译；在VC上的安装，主要是为了支持Visual studio的编译。由于libcstl发布的是源代码，最笨的方法就是直接将libcstl头文件和源文件的源代码加入工程就可以了，但对编译器的配置和需要编译的内容，相信读者是不愿意每次使用都从头来一次，更何况有些读者编译器方面知识的欠缺。所以安装成静态链接库lib和动态链接库dll是一劳永逸的事情。

## 安装MinGW和CygWin
MinGW是GNU for windows的项目，可以让GNU的项目在windows下运行，例如读者熟悉的gcc、make等；CygWin是一个在windows平台上运行的类UNIX模拟环境，可以运用读者熟悉的ls、rm... 等shell命令。这两个项目安装网上的教程很多，笔者就不再赘述。安装这两个项目的目的，就是为了把libcstl编译成\*.lib和\*.dll（libcstl有Makefile，可以减少读者自己配置编译的工作，而且读者自己编译多半不知道\*.c和*\.h之间的相互依赖关系，很容易出错）。

## 编译libcstl
安装好MinGW和CygWin以后，用CygWin进入libcstl-2.3.0\目录下，执行如下操作
```
\$ ./configure
\$ make
```
就会在libcstl-2.3.0\bin目录下生成\*.obj和\*.lib，\*.dll文件。熟悉编译原理的读者都知道，\*.obj是libcstl-2.3.0\src目录下\*.c文件生成的中间文件，也就是libcstl-2.3.0\cstl的\*.h文件的申明的实现，可以在编译阶段的链接阶段加入编译，而\*.lib，\*.dll相当于对\*.obj打包。具体的原理只是提一下，读者不必细究，只要知道通过make操作生成了有用的\*.lib，\*.dll文件就好。

## windows配置动态链接库libcstl.dll
将生成的libcstl.dll复制到C:\Windows\System32（32位系统）或者C:\Windows\SysWOW64（64位系统）下，如果不知道libcstl.dll的版本那就两个目录下都复制一份吧。

## GnuWin gcc配置静态链接库和头文件
复制libcstl-2.3.0\cstl目录到**MinGW安装目录\\include\\**文件夹下，这是gcc默认头文件的搜索目录。
复制libcstl.lib和libcstl.dll文件到**MinGW安装目录\\lib\\**文件夹下，这是gcc默认的静态链接库搜索目录。
这样就能在gcc下使用libcstl了，不要忘了在gcc编译的时候加上-l cstl，这是提醒编译器链接libcstl.dll文件，没有前面的lib三个字母，这是gcc的约定。或者可以在源代码中加入#pragma comment(lib,"libcstl.dll")这样提醒编译器的预处理命令。

## VC配置静态链接库和头文件
复制libcstl-2.3.0\cstl目录到**Visual studio安装目录\\VC\\include\\**文件夹下，这是VC默认头文件的搜索目录。
复制libcstl.lib文件到**Visual studio安装目录\\VC\\lib**文件夹下，这是VC默认的静态链接库搜索目录。
同样，在源代码中加入#pragma comment(lib,"libcstl.dll")这样提醒编译器的预处理命令。
