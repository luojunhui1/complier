/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-07 00:27:54
 * @LastEditTime: 2021-06-07 00:33:33
 */
#include "defs.h"
#include "data.h"
#include "decl.h"


/**
 * @brief Parse one or more statements
 * @param none
 * @return none
 * @details: 
 */
void statements(void) {
  struct ASTnode *tree;
  int reg;

  while (1) {
    // Match a 'print' as the first token
    match(T_PRINT, "print");

    // Parse the following expression and
    // generate the assembly code
    tree = binexpr(0);
    reg = genAST(tree);
    genPrintInt(reg);
    genFreeRegs();

    // Match the following semicolon
    // and stop if we are at EOF
    semi();
    destoryAST(tree);
    
    if (Token.token == T_EOF)
      return;
  }
}
