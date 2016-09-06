codecounter
===========
用于代码统计的命令行工具。该工具统计你的项目中源码（分类别统计：C，C++，java, Python...），
注释，以及空行的行数。

examples
----------
下图是统计的 linux 4.7 的源码树：

    $ ./codecounter /home/quuo/workspace/repos/linux/linux-stable

    ITEM:            COUNT   PERCENT
    assembly:     299937      0.01
    c:          11502091      0.57
    cpp:            1581      0.00
    header:      2845783      0.14
    python:         9471      0.00
    shell:          7237      0.00
    comment:     2888707      0.14
    blank:       2734755      0.13
    Totally:    20289562

usage
-----
1. `git clone https://github.com/quanzhuo/codecounter.git`
2. `cd codecounter`
3. `make`
4. `./codecounter your_project_path`
