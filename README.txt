/***************************************************************************
 * Description:
 * Author  : billy
 * Date    : 2012-09-07
 ***************************************************************************/

hello , 这里我的开发文档，当然并不是伴随这个开发过程 。只是我觉得有必要
将一些东西记下来，以便忘记。


1.使用java bochs调试
java -jar peter-bochs-debugger20110201.jar bochs -qf bochsrc1
可以运行java的程序来模拟执行bochs
じ

2.配置文件说明
dbochsrc  bochs 调试配置文件
gbochsrc  gdb+bochs调试配置文件

3.
运行make all 之前会将a.img先copy为2a.img然后再执行操作，这样出错后
可以'回滚'

4.虚拟地址管理
为了便于调试，我将fs,mm和init的虚拟地址设置为不一样的值，因为gdb调试时，地
址是虚拟地址

5.统计文件行数（单个文件）：

wc -l file

统计目录所有文件行数（全部目录）：

find . -name *.java | xargs wc -l


统计目录并按行数排序（按行大小排序）：

find . -name *.java | xargs wc -l | sort -n

统计目录并按行数排序（按行文件名排序）：

find . -name *.java | xargs wc -l | sort -k2

