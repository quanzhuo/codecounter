codecounter
===========
用于代码统计的命令行工具。该工具统计你的项目中源码（分类别统计：C，C++，java, Python...），
注释，以及空行的行数。

screenshot
----------
下图是统计的 linux 4.7 的源码树：
+ 总计 2002万行
+ 代码 1448万行
+ 空行 270万行
+ 注释 285万行

 ![linux](screenshot/linux.png)

 下图是统计的 Android 7.0 源码树：
 + 总计 7110 万行
 + 代码 5033 万行
 + 空行 707  万行
 + 注释 137  万行
 ![aosp](screenshot/aosp.png)

usage
-----
1. `git clone https://github.com/quanzhuo/codecounter.git`
2. `cd codecounter`
3. `make`
4. `./codecounter your_project_path`
