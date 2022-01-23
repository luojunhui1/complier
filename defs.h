/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:08:07
 * @LastEditTime: 2021-06-11 14:15:40
 */
#include <stdio.h>
#include <ctype.h>

#define TEXTLEN		512	// Length of symbols in input
#define NSYMBOLS        1024	// Number of symbol table entries
#define SYNTAX_ERROR    -1
#define SYNTAX_CORRECT    1
/* 
 * Token Types. The common term addition, subtraction, multiplication and division
 * is not used here because of the widespread reuse of operators in the design of 
 * C/C++ language
*/
enum {T_EOF, 
    T_CROSS, T_HORIZONTAL, T_ASTERISK, T_OBLIQUE, 
    T_EQ, T_NE,// == !=
    T_LT, T_GT, T_LE, T_GE,// < > <= >=
    T_INTLIT, T_SEMI, T_ASSIGN,T_IDENT,
    T_LBRACE, T_RBRACE, T_LPAREN, T_RPAREN,//{ , } , (, ) 
    //key words
    T_PRINT, T_CHAR, T_INT, T_IF, T_ELSE, T_WHILE, T_FOR, T_VOID  
};

//Token Structure
struct token
{
    int token;
    int intval;
};

// AST node types
enum {
  A_ADD = 1, A_SUBTRACT, A_MULTIPLY, A_DIVIDE, 
  A_EQ, A_NE, A_LT, A_GT, A_LE, A_GE,
  A_INTLIT,    
  A_IDENT, A_LVIDENT, A_ASSIGN, A_PRINT, A_GLUE,
  A_IF, A_WHILE, A_FUNCTION, A_WIDEN
};

// Abstract Syntax Tree structure
struct ASTnode {
  int op;				// "Operation" to be performed on this tree
  int type;     // Type of any expression this tree generates
  struct ASTnode *left;			// Left, Middle and right child trees
  struct ASTnode *mid;
  struct ASTnode *right;
  union 
  {
    int intvalue;				// For A_INTLIT, the integer value
    int id;             // For A_IDENT, the symbol slot number
  } v;
};

#define NOREG -1 // Use NOREG when the AST generation functions have no register to return

// Symbol table structure
struct symtable {
  char *name;			// Name of a symbol
  int type;                     // Primitive type for the symbol
  int stype;                    // Structural type for the symbol
};

// Primitive types
enum {
  P_NONE, P_VOID, P_CHAR, P_INT
};

// Structural types
enum {
  S_VARIABLE, S_FUNCTION
};