/*
 * @briefs: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-26 23:36:11
 * @LastEditTime: 2021-06-05 14:19:54
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "data.h"
#include "decl.h"

// Operator precedence for each token
static int OpPrec[] = { 0, 10, 10, 20, 20, 0 };

/**
 * @brief parse a primary factor and return an AST node to present it
 * @param none
 * @return an primary AST node pointer
**/
static struct ASTnode *primary(void)
{
    struct ASTnode *n;

    switch (Token.token)
    {
    case T_INTLIT:
        n = mkastleaf(A_INTLIT, Token.intval);
        scan(&Token);
        return n;    
    default:
        fprintf(stderr, "syntax error on line %d\n", Line);
    }
}

/**
 * @brief convert a Token into an AST node
 * @param tok type of token (Token.token)
 * @return AST node type  
**/
int arithop(int tok)
{
    switch(tok)
    {
        case T_CROSS:
            return (A_ADD);
        case T_HORIZONTAL:
            return (A_SUBTRACT);
        case T_ASTERISK:
            return (A_MULTIPLY);
        case T_OBLIQUE:
            return (A_DIVIDE);
        default:
            fprintf(stderr, "unknown token in arithop() on line %d\n", Line);
            exit(1);
    }
}

/**
 * @brief determines the precedence of the operator 
 * @param tokentype operator index
 * @return precedence of operator, higher the number , higher the precedence
 * @details the precedence table only for operator, when number or EOF imported, error arises  
 */
static int op_precedence(int tokentype)
{
    int prec = OpPrec[tokentype];

    if(prec == 0)
    {
        fprintf(stderr, "syntax error on line %d, token %d\n", Line, tokentype);
        exit(1);
    }

    return (prec);
}

/**
 * @brief build AST for a math expression only with binary operation and integer
 * @param ptp current operator precedence
 * @return AST node pointer
 */
struct ASTnode *binexpr(int ptp)
{
    struct ASTnode *n, *left, *right;

    int tokentype;
    
    // get the integer literal and  fetch the next token
    left = primary();

    // expression only has one integer literal
    tokentype = Token.token;
    if(tokentype == T_EOF)
        return left;

    //because the start precedence  
    while(op_precedence(tokentype) > ptp)
    {
        scan(&Token);

        right = (binexpr(OpPrec[tokentype]));

        left = mkastnode(arithop(tokentype), left, right, 0);

        tokentype = Token.token;

        if(tokentype == T_EOF)
            return (left);
    }       
    
    return (left);
}

/**
 * @brief: free memory allocated for AST
 * @param root AST node pointer
 * @return none
 * @details: 
 */
void destoryAST(struct ASTnode *root)
{
    if(root == NULL)
        return;
    
    destoryAST(root->left);
    destoryAST(root->right);

    {
        free(root);
        root = NULL;
        return;
    }
    
}