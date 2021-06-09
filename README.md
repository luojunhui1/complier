<!--
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-05 15:45:23
 * @LastEditTime: 2021-06-10 00:30:13
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
使用C语言编写的针对 `SubC` 可用的编译器，使用~~WPF~~`QT`来完成该编译器的GUI部分，以此来熟悉编译器在词法分析，语法分析，语义分析，优化操作层面上的工作原理，也会适当在原作者的代码中增加一些功能并为每个函数添加了友好的函数注释。目前来说，运行产生的汇编代码并不能生成在`×86-64`平台上可运行的目标文件，尽管目标如此，这可能是使用的汇编语法有误的原因。

# 工具
使用`vscode`进行代码撰写，`cmake`进行程序组织，`gdb`工具进行调试。

# 设计思想
主要使用`BNF`(巴斯克范式)作为文法的分析工具，使用`有限自动机`进行语法构造，而使用`AST`(抽象语法树)来进行自底向上的语法表达。

# 前端
前端太拉了，写好前端还是很有用的，这次不放。

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

## 自上而下：
自上而下在语法分析上直接面临着两个问题：
- 左递归
- 回溯

前者会导致语法的分析陷入无限的循环，即语法分析树不断增长，但分析却可能停滞；后者导致语法的分析过程中过了许多不必要的开销。前者有分为直接左递归和间接左递归，直接左递归的消除通过规范产生式的形式（将形如P->Pa|Pb|...|c|d的产生式转换为右递归文法）来完成，而对于间接左递归，通过对文法中的的所有非终结符进行排序，按序运行以下算法即可消除文法中的左递归

![](https://cdn.jsdelivr.net/gh/luojunhui1/BlogPicture/Windows/20210609171151.png)
而对于回溯的消除，借助着朴素的思想，当一个句型中的非终结符要向下匹配时，若它`所有可能的推导的首字符均不相同`，并且要匹配的句子的下一个字符刚好出现在其中，那么我们选择对应的那个推导就必然不会出错，这就是`FIRST集合`的目的所在，若要匹配的句子的下一个字符不出现在该句型的所有推导的首字符中，那么这个句型必然是\epsilon或者推导的句子有误，为正确地选择对应的推导，需要知道对于当前非终结符接下来第一个出现的所有可能字符（当前非终结符为\epsilon）,选择其中首字符为句子的下一个字符的推导，这就是`FOLLOW集合`的目的所在。在句型满足`以上条件`并前`按以上过程操作`后，就可消除推导过程中的回溯。
满足该条件的文法就是LL(1)文法。同时，`LL(1)文法也是无二义文法`。

对于自上而下的分析器，在实现上选择`递归下降子程序`来进行语法分析，它的结构通常如下：
![](https://cdn.jsdelivr.net/gh/luojunhui1/BlogPicture/Windows/20210609180122.png)

## 自下而上：
 