/*
 * @Descriptions: Code generator for x86-64
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-05 17:25:12
 * @LastEditTime: 2021-06-05 17:57:41
 */
#include <stdlib.h>

#include "defs.h"
#include "data.h"
#include "decl.h"

// List of available registers and their names
static int freereg[4];
static char *reglist[4]= { "%r8", "%r9", "%r10", "%r11" };

/**
 * @brief free all common registers for x86-64 plateform
 * @param none
 * @return none
 * @details: 
 */
void freeAllRegisters(void)
{
    freereg[0] = freereg[1] = freereg[2] = freereg[3] = 1;
}

/**
 * @brief allocate register from free registers
 * @param none
 * @return allocated register
 * @details: only works for single thread program
 */
static int allocRegister(void)
{
    for(int i = 0; i < 4; i++)
        if(freereg[i])
        {
            freereg[i] = 0;
            return (i);
        }
    fprintf(stderr, "Out of Registers!\n");
    exit(1);
}

/**
 * @brief free register
 * @param reg register to be released
 * @return none
 * @details: 
 */
static void freeRegister(int reg)
{
  if (freereg[reg] != 0) {
    fprintf(stderr, "Error trying to free register %d\n", reg);
    exit(1);
  }
  freereg[reg]= 1;
}

/**
 * @brief Print out the assembly preamble
 * @param none
 * @return none
 * @details: 
 */
void cgPreamble()
{
  freeAllRegisters();
  fputs(
	"\t.text\n"
	".LC0:\n"
	"\t.string\t\"%d\\n\"\n"
	"printint:\n"
	"\tpushq\t%rbp\n"
	"\tmovq\t%rsp, %rbp\n"
	"\tsubq\t$16, %rsp\n"
	"\tmovl\t%edi, -4(%rbp)\n"
	"\tmovl\t-4(%rbp), %eax\n"
	"\tmovl\t%eax, %esi\n"
	"\tleaq	.LC0(%rip), %rdi\n"
	"\tmovl	$0, %eax\n"
	"\tcall	printf@PLT\n"
	"\tnop\n"
	"\tleave\n"
	"\tret\n"
	"\n"
	"\t.globl\tmain\n"
	"\t.type\tmain, @function\n"
	"main:\n"
	"\tpushq\t%rbp\n"
	"\tmovq	%rsp, %rbp\n",
  outFile);
}

/**
 * @brief Print out the assembly postamble
 * @param none
 * @return none
 * @details: 
 */
void cgPostamble()
{
  fputs(
	"\tmovl	$0, %eax\n"
	"\tpopq	%rbp\n"
	"\tret\n",
  outFile);
}

/**
 * @brief: Load an integer literal value into a register.
 * @param value integer value to be introduced into a register
 * @return Return the number of the register
 * @details: 
 */
int cgLoad(int value) {

  // Get a new register
  int r= allocRegister();

  // Print out the code to initialise it
  fprintf(outFile, "\tmovq\t$%d, %s\n", value, reglist[r]);
  return(r);
}

/**
 * @brief Add two registers together
 * @param r1
 * @param r2
 * @return  return the number of the register with the result
 * @details: 
 */
int cgAdd(int r1, int r2) {
  fprintf(outFile, "\taddq\t%s, %s\n", reglist[r1], reglist[r2]);
  freeRegister(r1);
  return(r2);
}

/**
 * @brief Subtract the second register from the first
 * @param r1
 * @param r2
 * @return return the number of the register with the result
 * @details: 
 */
int cgSub(int r1, int r2) {
  fprintf(outFile, "\tsubq\t%s, %s\n", reglist[r2], reglist[r1]);
  freeRegister(r2);
  return(r1);
}

/**
 * @brief Multiply two registers together
 * @param r1
 * @param r2
 * @return the number of the register with the result
 * @details: 
 */
int cgMul(int r1, int r2) {
  fprintf(outFile, "\timulq\t%s, %s\n", reglist[r1], reglist[r2]);
  freeRegister(r1);
  return(r2);
}

/**
 * @brief Divide the first register by the second
 * @param r1
 * @param r2
 * @return return the number of the register with the result
 * @details: 
 */
int cgDiv(int r1, int r2) {
  fprintf(outFile, "\tmovq\t%s,%%rax\n", reglist[r1]);
  fprintf(outFile, "\tcqo\n");
  fprintf(outFile, "\tidivq\t%s\n", reglist[r2]);
  fprintf(outFile, "\tmovq\t%%rax,%s\n", reglist[r1]);
  freeRegister(r2);
  return(r1);
}

/**
 * @brief Call printint() with the given register
 * @param r
 * @return none
 * @details: 
 */
void cgPrintInt(int r) {
  fprintf(outFile, "\tmovq\t%s, %%rdi\n", reglist[r]);
  fprintf(outFile, "\tcall\tprintint\n");
  freeRegister(r);
}
