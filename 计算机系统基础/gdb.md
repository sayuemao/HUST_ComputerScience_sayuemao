gdb是Linux下的调试工具，可用来调试C语言、C++语言、汇编语言编写生成的可执行程序。

# 格式
命令格式：
gdb \[option\] \[可执行程序\]

常用option:
--args Arguments    Arguments作为传递给可执行程序的参数

# gdb使用过程
Linux下，汇编、链接和调试过程：
+ as -g --32 -o hello.o hello.s
+ ld -m elf-i386  -o hello hello.o
+ gdb hello

Linux下，编译、调试过程：
+ gcc -g -m32 -o hello hello.c
+ gdb hello

# gdb使用方法
进入gdb后，可以输入命令进行调试。

设置断点：
+ b

执行：
+ r
+ c
+ n
- ni
- s
- si

查看：
- info r
- x 地址， x/32bx 地址

反汇编：
- disass /m
