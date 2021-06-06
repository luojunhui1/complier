/*
 * @Descriptions: Generic code generator
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-05 17:09:23
 * @LastEditTime: 2021-06-07 00:37:43
 */

#include <stdlib.h>

#include "defs.h"
#include "data.h"
#include "decl.h"

/**
 * @brief generate assembly code from AST
 * @param n root node of AST
 * @return current result of math operation
 * @details: the reason that this function can works is the AST is build by precedence, so the operation that each operator node
 * only use its left node value and right node value to caculate its own value can make sense. 
 */
int genAST(struct ASTnode *n)
{
    int left_reg, right_reg;

    if(n->left)
        left_reg = genAST(n->left);
    if(n->right)
        right_reg = genAST(n->right);
    
    switch (n->op)
    {
    case A_ADD:
        return cgAdd(left_reg, right_reg);
    case A_SUBTRACT:
        return cgSub(left_reg, right_reg);
    case A_MULTIPLY:
        return cgMul(left_reg, right_reg);
    case A_DIVIDE:
        return cgDiv(left_reg, right_reg);
    case A_INTLIT:
        return cgLoad(n->intvalue);
    default:
        fprintf(stderr, "Unknow AST operator %d\n", n->op);
        exit(1);
    }
}

void genPreamble() {
  cgPreamble();
}

void genPostamble() {
  cgPostamble();
}

void genFreeRegs() {
  freeAllRegisters();
}

void genPrintInt(int reg) {
  cgPrintInt(reg);
}


void generateCode(struct ASTnode *n)
{
    int reg;

    cgPreamble();
    reg = genAST(n);
    cgPrintInt(reg);
    cgPostamble();
    
}