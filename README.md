<!--
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-05 15:45:23
 * @LastEditTime: 2021-06-13 19:05:22
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
主要使用`BNF`(巴斯克范式)作为文法的分析工具，使用`有限自动机`进行语法构造，而使用`AST`(抽象语法树)来进行自底向上的语法表达。

# 前端
前端太拉了，写好前端还是很有用的，这次不把前端代码上传。

# 使用
在cmake和其它环境配置好了的情况下，只要在项目目录下命令行中键入
> ./run

即可，但实际使用该方法需要注意run.bat中的内容，./run后出现的提示输入的文件名均以项目目录为根目录，输入文件默认为input6,输出文件默认为输入文件加上.words和.out后缀。
若要将本程序编译为动态库以供python或.net、java使用的话，需将CMakeLists.txt更改为如下：
```
cmake_minimum_required(VERSION 3.0.0) 

project(lComplier VERSION 0.1.1 LANGUAGES C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set(CMAKE_BUILD_TYPE RELEASE)

SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib)
include_directories(${PROJECT_SOURCE_DIR}/include)


file(GLOB SOURCES "*.c")
add_library(${PROJECT_NAME} SHARED ${SOURCES})

install(TARGETS lComplier
        )

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINKER_LANGUAGE C
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} 
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)
```
并根据需求更改main函数，编译后 `make install`即可使用该库，需注意，在windows平台下需使用管理员权限打开命令行运行 `make install`命令才可在`C:\Program Files (x86)`中创建相应的动态库。
WPF使用该动态库的方式如下：
``` 
 [DllImport("liblComplier.dll", EntryPoint = "analysis", CallingConvention = CallingConvention.Cdecl)]
 
public static extern int analysis();
```

# 参考
https://github.com/DoctorWkt/acwj 

https://en.cppreference.com/w/c/language/operator_precedence

# 杂
编程语言中所有类别的的statement（关键字，赋值，IF判断, WHILE循环，比较， 四则运算等等）均可以用正规式表达，正规式也可以使用NFA来进行表示，即使对于正规式之间的结合，如将赋值语句与比较语句相结合（Variable = A > B），也可以使用NFA来表示，因为NFA各个状态之间是可以通过正规式到达的，而NFA又可以转换为对应的DFA，DFA可以方便地使用程序来表示，这就为编译程序的实现提供了理论基础。

从NFA到DFA主要有以下几个矛盾：
- 初态、终态不唯一： 通过加上统一的终态初态解决
- NFA中边可为$\epsilon$ 、字和正规式： 通过闭包解决，使每个DFA中的节点代表一个闭包（子集法）
- NFA的不确定性： 通过上一个方法一并解决

再将NFA转化为DFA之后，再将DFA中的状态集划分为几个闭包，将到达终态所有路径相同的状态合并为一个状态，以此简化DFA中的状态数目，这一过程通过递归地计算集合中的闭包来完成。（最小化算法）

## 自上而下
自上而下指从文法的开始符号开始，反复使用各种产生式，寻找匹配的**推导**。

自上而下在语法分析上直接面临着两个问题：
- 左递归
- 回溯

前者会导致语法的分析陷入无限的循环，即语法分析树不断增长，但分析却可能停滞；后者导致语法的分析过程中过了许多不必要的开销。前者有分为直接左递归和间接左递归，直接左递归的消除通过规范产生式的形式（将形如P->Pa|Pb|...|c|d的产生式转换为右递归文法）来完成，而对于间接左递归，通过对文法中的的所有非终结符进行排序，按序运行以下算法即可消除文法中的左递归

![](https://cdn.jsdelivr.net/gh/luojunhui1/BlogPicture/Windows/20210609171151.png)
而对于回溯的消除，借助着朴素的思想，当一个句型中的非终结符要向下匹配时，若它`所有可能的推导的首字符均不相同`，并且要匹配的句子的下一个字符刚好出现在其中，那么我们选择对应的那个推导就必然不会出错，这就是`FIRST集合`的目的所在，若要匹配的句子的下一个字符不出现在该句型的所有推导的首字符中，那么这个句型必然是\epsilon或者推导的句子有误，为正确地选择对应的推导，需要知道对于当前非终结符接下来第一个出现的所有可能字符（当前非终结符为$\epsilon$）,选择其中首字符为句子的下一个字符的推导，这就是`FOLLOW集合`的目的所在。在句型满足`以上条件`并前`按以上过程操作`后，就可消除推导过程中的回溯。
满足该条件的文法就是LL(1)文法。同时，`LL(1)文法也是无二义文法`。

对于自上而下的分析器，在实现上选择`递归下降子程序`来进行语法分析，它的结构通常如下：
![](https://cdn.jsdelivr.net/gh/luojunhui1/BlogPicture/Windows/20210609180122.png)

## 自下而上
自下而上指从输入串开始，逐步进行**规约**，直到文法的开始符号。基本思想是：

> 用一个寄存符号的先进后出栈，把输入符号一个一个地移进到栈里，当栈顶形成某个产生式的候选式时，即把栈顶的这一部分替换成(归约为)该产生式的左部符号。

核心问题是`识别可规约串`，同时需注意到，自下而上语法中由规约产生的树形结构在内容上与语法树可能并不一致，其也被称为`分析树`。

在自下而上语法中出现了许多定义(以下不说明在形式语言上的定义，而只说明判断方法)：

-  `短语`: 两代以上子树所有的末端节点从左到右排列
-  `直接短语`: 两代子树的短语
-  `算符文法`: 一个文法，若其任意产生式的右部都不含两个相继的非终结符即为算符文法
-  `算符优先文法`: 任意两个非终结符之间只满足$\lessdot \gtrdot =（中间应有一实心点）$之间的一个
- `a等优先b`：当且仅当文法G中含有形如P:arrow_right:…ab…或P:arrow_right:…aQb…的产生式
- `b右优先a`：当且仅当文法G中含有形如P:arrow_right:…aR…的产生式，而R可直接推导出b…或Qb…
- `a左优先b`：当且仅当文法G中含有形如P:arrow_right:…Rb…的产生式，而R可直接推导出…a或…aQ
- `FIRSTVT(P)`:  文法中非终结符P可直接推导出的产生式右部的第一个终结符
-  `LASTVT(P)`:  文法中非终结符P可直接推导出的产生式右部的最后一个终结符
-  `素短语`： 指这样一个短语，它至少含有一个终结符，并且，除它自身之外不再含任何更小的素短语
-  `最左素短语`： 指处于句型最左边的那个素短语
-  `句柄`： 最左两代子树的末端就是句柄
- `前缀`： 字的任意首部
- `活前缀`： 规范句型的一个前缀，不含句柄之后的任何符号。（在LR分析中分析栈内的符号始终构成活前缀）
- `LR(0)项目`： 在每个惨胜十右部添加一个小圆点表示在分析过程中看到了产生式的多大部分


### 算符优先文法

通过构造出文法中每个非终结符对应的FIRSTVT和LASTVT表，进而获得文法中每个非终结符之间的优先关系获得优先级关系表，通过最**左素短语定理**和优先关系表找出符号串中的最左素短语，这一过程在程序上是如此实现的

> 不断从符号串中读入符号移入分析栈中，直到新读入的符号优先级的优先级低于上一次读入符号，此时上一次读入的符号就是最左素短语的最后一个终结符，然后在符号串中向前规约即可得到最左素短语。

并且，由于在归约时，算符优先文法指要求终结符对应终结符严格相同而非终结符只要位置对应即可，故算符分析文法所生成的分析树与语法树并不一定一致。

**优点**：速度快

**缺点**： 可能错误接受非法句子 

### LR分析法

LR分析法通过不断寻找句柄规约来进行语法分析，寻找句柄成为了LR分析发的核心，这一任务通过LR分析表来完成，通过ACTION(s,a)和GOTO(s,X)不断前进，最终实现对整个符号串进行规约。

对于一个文法，如果能够构造一张分析表，使得它的每个入口均是唯一确定的，则这个文法就称为LR文法。

一个文法，如果能用一个每步顶多向前检查k个输入符号的LR分析器进行分析，则这个文法就称为LR(k)文法。LR(1)的分析过程是一个规范归约的过程，对于规范归约来说，符号栈和未归约的符号串始终构成一个句型，并且被归约的句柄始终出现在符号栈的栈顶。
