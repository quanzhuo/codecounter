codecounter
===========
一个简单粗糙的统计代码行数的命令行工具。该工具统计你的项目中源码（分类别统计：C，C++，
java, Python...），注释，以及空行的行数。对于注释，它只统计独占一行的注释，嵌入代码行
的注释不统计。

examples
----------
下面是统计的 linux 4.7 的源码树：

    $ ./codecounter /home/quuo/workspace/repos/linux/linux-stable

    ITEM:            COUNT   PERCENT
    assembly:     300086      0.01
    c:          11501206      0.55
    cpp:            1581      0.00
    header:      2846257      0.14
    python:         9471      0.00
    shell:          7247      0.00
    makefile:      34607      0.00
    comment:     2896861      0.14
    plaintext:    431877      0.02
    blank:       2742821      0.13
    Totally:    20772014


下面是统计的 Aosp 项目的源码统计：

    $ ./codecounter /home/quuo/workspace/repos/aosp

    ITEM:            COUNT   PERCENT
    assembly:    1516892      0.01
    c:           9785389      0.09
    cpp:        10404029      0.10
    header:     25080282      0.24
    java:        8261783      0.08
    python:      3351773      0.03
    go:          2135812      0.02
    shell:        269525      0.00
    makefile:     212808      0.00
    xml:        10158021      0.10
    comment:    15889275      0.15
    plaintext:  10111251      0.10
    blank:       8913566      0.08
    Totally:   106090406


how to build
-----
1. `git clone https://github.com/quanzhuo/codecounter.git`
2. `cd codecounter`
3. `make`

usage
-----
`$ ./codecounter your_project_path`
