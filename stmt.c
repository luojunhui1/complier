/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-07 00:27:54
 * @LastEditTime: 2021-06-07 01:59:31
 */
#include "defs.h"
#include "data.h"
#include "decl.h"

void printStatement(void) {
  struct ASTnode *tree;
  int reg;

  // Match a 'print' as the first token
  match(T_PRINT, "print");

  // Parse the following expression and
  // generate the assembly code
  tree = binexpr(0);
  reg = genAST(tree, -1);
  genPrintInt(reg);
  genFreeRegs();

  // Match the following semicolon
  semi();
}

void assignmentStatement(void) {
  struct ASTnode *left, *right, *tree;
  int id;

  // Ensure we have an identifier
  ident();

  // Check it's been defined then make a leaf node for it
  if ((id = findGlob(Text)) == -1) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(A_LVIDENT, id);

  // Ensure we have an equals sign
  match(T_EQUALS, "=");

  // Parse the following expression
  left = binexpr(0);

  // Make an assignment AST tree
  tree = mkastnode(A_ASSIGN, left, right, 0);

  // Generate the assembly code for the assignment
  genAST(tree, -1);
  genFreeRegs();

  // Match the following semicolon
  semi();
}


/**
 * @brief Parse one or more statements
 * @param none
 * @return none
 * @details: 
 */
void statements(void) {
  while (1) {
    switch (Token.token)
    {
    case T_PRINT:
        printStatement();
        break;
    case T_INT:
        varDeclaration();
        break;
    case T_IDENT:
        assignmentStatement();
        break;
    case T_EOF:
        return;
    default:
        fatald("Syntax error, token", Token.token);
    }
  }
}
