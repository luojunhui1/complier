/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:08:07
 * @LastEditTime: 2021-06-07 00:18:26
 */
#include <stdio.h>
#include <ctype.h>


#define TEXTLEN		512	// Length of symbols in input

/* 
 * Token Types. The common term addition, subtraction, multiplication and division
 * is not used here because of the widespread reuse of operators in the design of 
 * C/C++ language
*/
enum {T_EOF, T_CROSS, T_HORIZONTAL, T_ASTERISK, T_OBLIQUE, T_INTLIT, T_SEMI, T_PRINT};

//Token Structure
struct token
{
    int token;
    int intval;
};

// AST node types
enum {
  A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT
};

// Abstract Syntax Tree structure
struct ASTnode {
  int op;				// "Operation" to be performed on this tree
  struct ASTnode *left;			// Left and right child trees
  struct ASTnode *right;
  int intvalue;				// For A_INTLIT, the integer value
};