/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:08:07
 * @LastEditTime: 2021-06-07 01:50:41
 */
#include <stdio.h>
#include <ctype.h>


#define TEXTLEN		512	// Length of symbols in input
#define NSYMBOLS        1024	// Number of symbol table entries

/* 
 * Token Types. The common term addition, subtraction, multiplication and division
 * is not used here because of the widespread reuse of operators in the design of 
 * C/C++ language
*/
enum {T_EOF, T_CROSS, T_HORIZONTAL, T_ASTERISK, T_OBLIQUE, T_INTLIT, T_SEMI, T_EQUALS,
    T_IDENT,
    //key words
    T_PRINT, T_INT};

//Token Structure
struct token
{
    int token;
    int intval;
};

// AST node types
enum {
  A_ADD, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, A_INTLIT,A_IDENT, A_LVIDENT, A_ASSIGN
};

// Abstract Syntax Tree structure
struct ASTnode {
  int op;				// "Operation" to be performed on this tree
  struct ASTnode *left;			// Left and right child trees
  struct ASTnode *right;
  union 
  {
    int intvalue;				// For A_INTLIT, the integer value
    int id;             // For A_IDENT, the symbol slot number
  } v;
};

// Symbol table structure
struct symtable {
  char *name;			// Name of a symbol
};