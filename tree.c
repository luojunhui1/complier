/*
 * @briefs: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-27 00:11:24
 * @LastEditTime: 2021-06-07 14:59:03
 */
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "data.h"
#include "decl.h"

/**
 * @brief build and return a generic AST node
 * @param op AST node operation type
 * @param left left-hand node pointer
 * @param right right-hand node pointer
 * @param value AST node's integer literal value  
 * @return AST node pointer
**/
struct ASTnode *mkastnode(int op, struct ASTnode *left, struct ASTnode *right, int value)
{
    struct ASTnode *n;

    n = (struct ASTnode *)malloc(sizeof(struct ASTnode));

    if(n == NULL)
    {
      fatal("unable to malloc in mkastnode()\n");
    }

    n->op = op;
    n->left = left;
    n->right = right;
    n->v.intvalue = value;

    return (n);
}

/**
 * @brief build and return a AST node with integer literal value
 * @param op AST node type
 * @param intvalue integer literal value
 * @return AST node pointer
**/
struct ASTnode *mkastleaf(int op, int intvalue) {
  return (mkastnode(op, NULL, NULL, intvalue));
}

/**
 * @brief make a unary AST node: only one child
 * @param op  AST node type
 * @param left left-hand node pointer
 * @param intvalue integer literal value
 * @return AST node pointer
**/
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue) {
  return (mkastnode(op, left, NULL, intvalue));
}




