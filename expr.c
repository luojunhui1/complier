/*
 * @briefs: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-26 23:36:11
 * @LastEditTime: 2021-06-11 15:01:57
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
static struct ASTnode *primary(int *flag)
{
    struct ASTnode *n;
    int id;
    
    // *flag = SYNTAX_CORRECT;
    switch (Token.token)
    {
    case T_INTLIT:
        if ((Token.intval) >= 0 && (Token.intval < 256))
            n = mkastleaf(A_INTLIT, P_CHAR, Token.intval);
        else
            n = mkastleaf(A_INTLIT, P_INT, Token.intval);
        break;
    case T_IDENT:
        id = findGlob(Text);
        if(id == -1)
        {
            fatals("Unknown variable", Text);  
            *flag = SYNTAX_ERROR;
        }
        n = mkastleaf(A_IDENT, Gsym[id].type, id);
        break;
    default:
    {
        fatald("Syntax error, token", Token.token);
        *flag = SYNTAX_ERROR;
    }
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
int arithop(int tok, int *flag)
{
    // *flag = SYNTAX_CORRECT;

    if(tok < T_INTLIT && tok > T_EOF)
        return tok;
        
    fatald("Syntax error, token", tok);
    *flag = SYNTAX_ERROR;;
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
        return SYNTAX_ERROR;
    }

    return (prec);
}

/**
 * @brief build AST for a math expression only with binary operation and integer
 * @param ptp current operator precedence
 * @return AST node pointer
 */
struct ASTnode *binexpr(int ptp, int *flag)
{
    struct ASTnode *n, *left, *right;
    int lefttype, righttype;
    int tokentype;
    
    // get the integer literal and  fetch the next token
    left = primary(flag);
    if(*flag == SYNTAX_ERROR)
        return NULL;

    // when the scanner hit the ',' or ')', just return a left node
    tokentype = Token.token;
    if(tokentype == T_SEMI || tokentype == T_RPAREN)
        return left;

    //because the start precedence  
    while(op_precedence(tokentype) > ptp)
    {
        scan(&Token);

        right = (binexpr(OpPrec[tokentype], flag));
        
        lefttype = left->type;
        righttype = right->type;

        if (!typeCompatible(&lefttype, &righttype, 0))
            fatal("Incompatible types");

        // Widen either side if required. type vars are A_WIDEN now
        if (lefttype)
            left = mkastunary(lefttype, right->type, left, 0);
        if (righttype)
            right = mkastunary(righttype, left->type, right, 0);

        // Join that sub-tree with ours. Convert the token
        // into an AST operation at the same time.
        left = mkastnode(arithop(tokentype, flag), left->type, left, NULL, right, 0);

        if(*flag == SYNTAX_ERROR)
            return NULL;

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