/*
 * @briefs: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-26 23:36:11
 * @LastEditTime: 2021-06-08 12:56:27
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "data.h"
#include "decl.h"

// Operator precedence for each token
static int OpPrec[] = { 0, //EOF
                        10, 10, 20, 20,// + - * /
                        30, 30,        // == !=
                        40, 40, 40, 40, // < > <= >=
                        0
};

/**
 * @brief parse a primary factor and return an AST node to present it
 * @param none
 * @return an primary AST node pointer
**/
static struct ASTnode *primary(void)
{
    struct ASTnode *n;
    int id;
    
    switch (Token.token)
    {
    case T_INTLIT:
        n = mkastleaf(A_INTLIT, Token.intval);
        break;
    case T_IDENT:
        id = findGlob(Text);
        if(id == -1)
            fatals("Unknown variable", Text);  
        n = mkastleaf(A_IDENT, id);
        break;
    default:
        fatald("Syntax error, token", Token.token);
    }

    scan(&Token);
    return n;
}

/**
 * @brief convert a Token into an AST node
 * @param tok type of token (Token.token)
 * @return AST node type  
 * @details now the convertion from token type to AST node type relyinh on a 1:1 mapping from token to AST operation,
 * so this function only used to determine whether the token type is vaild or not
**/
int arithop(int tok)
{
    if(tok < T_INTLIT && tok > T_EOF)
        return tok;
    fatald("Syntax error, token", tok);
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

    // when the scanner hit the ',' or ')', just return a left node
    tokentype = Token.token;
    if(tokentype == T_SEMI || tokentype == T_RPAREN)
        return left;

    //because the start precedence  
    while(op_precedence(tokentype) > ptp)
    {
        scan(&Token);

        right = (binexpr(OpPrec[tokentype]));

        left = mkastnode(arithop(tokentype), left, NULL, right, 0);

        tokentype = Token.token;

        if(tokentype == T_SEMI || tokentype == T_RPAREN)
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