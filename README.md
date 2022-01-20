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

