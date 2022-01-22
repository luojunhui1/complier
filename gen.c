/*
 * @Descriptions: Generic code generator
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-05 17:09:23
 * @LastEditTime: 2021-06-08 16:08:41
 */

#include <stdlib.h>

#include "defs.h"
#include "data.h"
#include "decl.h"


// Generate and return a new label number
static int label(void) {
  static int id = 1;
  return (id++);
}

// Generate the code for an IF statement and an optional ELSE clause
static int genIFAST(struct ASTnode *n) {
  int Lfalse, Lend;

  // Generate two labels: one for the
  // false compound statement, and one
  // for the end of the overall IF statement.
  // When there is no ELSE clause, Lfalse _is_
  // the ending label!
  Lfalse = label();
  if (n->right)
    Lend = label();

  // Generate the condition code followed
  // by a zero jump to the false label.
  // We cheat by sending the Lfalse label as a register.
  genAST(n->left, Lfalse, n->op);
  genFreeRegs();

  // Generate the true compound statement
  genAST(n->mid, NOREG, n->op);
  genFreeRegs();

  // If there is an optional ELSE clause, generate the jump to skip to the end
  if (n->right)
    cgJump(Lend);

  // Now the false label
  cgLabel(Lfalse);

  // Optional ELSE clause: generate the false compound statement and the end label
  if (n->right) {
    genAST(n->right, NOREG, n->op);
    genFreeRegs();
    cgLabel(Lend);
  }

  return (NOREG);
}

/**
 * @brief Generate the code for a WHILE statement and an optional ELSE clause
 * @param n root node of AST
 * @return -1
 * @details: 
 */
static int genWHILE(struct ASTnode *n) {
  int Lstart, Lend;

  // Generate the start and end labels
  // and output the start label
  Lstart = label();
  Lend = label();
  cgLabel(Lstart);

  // Generate the condition code followed
  // by a jump to the end label.
  // We cheat by sending the Lfalse label as a register.
  genAST(n->left, Lend, n->op);
  genFreeRegs();

  // Generate the compound statement for the body
  genAST(n->right, NOREG, n->op);
  genFreeRegs();

  // Finally output the jump back to the condition,
  // and the end label
  cgJump(Lstart);
  cgLabel(Lend);
  return (NOREG);
}

/**
 * @brief generate assembly code from AST
 * @param n root node of AST
 * @param reg the register id stores the final value of current node
 * @return current result of math operation
 * @details: the reason that this function can works is the AST is build by precedence, so the operation that each operator node
 * only use its left node value and right node value to caculate its own value can make sense. 
 */
int genAST(struct ASTnode *n, int reg, int parentASTop)
{
    switch (n->op)
    {
    case A_IF:
      return (genIFAST(n));
        break;
    case A_WHILE:
        return (genWHILE(n));
        break;
    case A_GLUE:
      // Do each child statement, and free the registers after each child
      genAST(n->left, NOREG, n->op);
      genFreeRegs();
      genAST(n->right, NOREG, n->op);
      genFreeRegs();
      return (NOREG);
    case A_FUNCTION:
      cgFuncPreamble(Gsym[n->v.id].name);
      genAST(n->left, NOREG, n->op);
      cgFuncPostamble();
      return (NOREG);
    }

    int left_reg, right_reg;

    if(n->left)
        left_reg = genAST(n->left, NOREG, n->op);
    if(n->right)
        right_reg = genAST(n->right, left_reg, n->op);
    
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
    case A_EQ:
    case A_NE:
    case A_LT:
    case A_GT:
    case A_LE:
    case A_GE:
        if(parentASTop == A_IF)
            return (cgCompareAndJump(n->op, left_reg, right_reg, reg));
        else
            return (cgCompareAndSet(n->op, left_reg, right_reg));
    case A_INTLIT:
        return cgLoadInt(n->v.intvalue);
    case A_IDENT:
        return (cgLoadGlob(Gsym[n->v.id].name));
    case A_LVIDENT:
        return (cgStorGlob(reg, Gsym[n->v.id].name));
    case A_ASSIGN:
    // The work has already been done, return the result
        return (right_reg);
    case A_PRINT:
      // Print the left-child's value
      // and return no register
      genPrintInt(left_reg);
      genFreeRegs();
      return (NOREG);
    default:
        fprintf(stderr, "Unknow AST operator %d\n", n->op);
        return (1);
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

void genGlobSym(char *s) {
  cgGlobSym(s);
}