/*
 * @briefs: interpret abstract syntax tree
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-03 11:19:11
 * @LastEditTime: 2021-06-03 11:43:20
 */
#include <stdlib.h>

#include "defs.h"
#include "data.h"
#include "decl.h"

// List of AST operators
static char *ASTop[] = { "+", "-", "*", "/" };

/**
 * @brief  interpret abstract syntax tree
 * @param n AST node pointer
 * @return integer literal value after present operation
**/
int interpretAST(struct ASTnode *n) {
  int leftval, rightval;

  // Get the left and right sub-tree values
  if (n->left)
    leftval = interpretAST(n->left);
  if (n->right)
    rightval = interpretAST(n->right);

  // Debug: Print what we are about to do
  if (n->op == A_INTLIT)
    printf("int %d\n", n->intvalue);
  else
    printf("%d %s %d\n", leftval, ASTop[n->op], rightval);

  switch (n->op) {
    case A_ADD:
      return (leftval + rightval);
    case A_SUBTRACT:
      return (leftval - rightval);
    case A_MULTIPLY:
      return (leftval * rightval);
    case A_DIVIDE:
      return (leftval / rightval);
    case A_INTLIT:
      return (n->intvalue);
    default:
      fprintf(stderr, "Unknown AST operator %d\n", n->op);
      exit(1);
  }
}
