<!--
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-05 15:45:23
 * @LastEditTime: 2021-06-08 15:12:30
-->
# COMPLIER
<p align="center">
    <img alt="last-update" src="https://img.shields.io/github/last-commit/luojunhui1/complier?label=Code%20Update&logo=visual-studio-code"></img>
    <img alt="csharp-version" src="https://img.shields.io/badge/C%23-8.0-blue.svg"></img>
    <img alt="IDE-version" src="https://img.shields.io/badge/IDE-vs2019-blue.svg"></img>
    <img alt="cmake-version" src="https://img.shields.io/badge/CMake-3.17.3-blue.svg"></img>
    <img alt="Lisense" src="https://img.shields.io/badge/Lisense-GPL3-green.svg"></img>
</p>

# 简介
使用C语言编写的针对 `SubC` 可用的编译器，使用`WPF`来完成该编译器的GUI部分，以此来熟悉编译器在词法分析，语法分析，语义分析，优化操作层面上的工作原理，也会适当在原作者的代码中增加一些功能并为每个函数添加了友好的函数注释。目前来说，运行产生的汇编代码并不能生成在`×86-64`平台上可运行的目标文件，尽管目标如此，这可能是使用的汇编语法有误的原因。

# 工具
使用`vscode`进行代码撰写，`cmake`进行程序组织，`gdb`工具进行调试。

# 设计思想
主要使用`BNF`(巴斯克范式)作为文法的分析工具，而使用`AST`(抽象语法数)来进行自底向上的语法构造,有限自动机进行语法分析。

# 参考
https://github.com/DoctorWkt/acwj 

https://en.cppreference.com/w/c/language/operator_precedence

# 杂
编程语言中所有类别的的statement（关键字，赋值，IF判断, WHILE循环，比较， 四则运算等等）均可以用正规式表达，正规式也可以使用NFA来进行表示，即使对于正规式之间的结合，如将赋值语句与比较语句相结合（Variable = A > B），也可以使用NFA来表示，因为NFA各个状态之间是可以通过正规式到达的，而NFA又可以转换为对应的DFA，DFA可以方便地使用程序来表示，这就为编译程序的实现提供了理论基础。

从NFA到DFA主要有以下几个矛盾：
- 初态、终态不唯一： 通过加上统一的终态初态解决
- NFA中边可为\epsilon 、字和正规式： 通过闭包解决，使每个DFA中的节点代表一个闭包（子集法）
- NFA的不确定性： 通过上一个方法一并解决

再将NFA转化为DFA之后，再将DFA中的状态集划分为几个闭包，将到达终态所有路径相同的状态合并为一个状态，以此简化DFA中的状态数目，这一过程通过递归地计算集合中的闭包来完成。（最小化算法）



 

