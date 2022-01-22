/*
 * @briefs: main process
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 21:07:22
 * @LastEditTime: 2021-06-11 15:03:03
 */

#include "defs.h"
#define extern_
    #include "data.h"
#undef extern_
#include "decl.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

static void init()
{
    Line = 1;
    Putback = '\0';
}

static int usage(char *prog)
{
    fprintf(stderr, "Usage : %s dir_of_input_file\n", prog);
    return (1);
}

char *tokstr[] = {"+", "-", "*", "/", "intlit"};


int main(int argc, char *argv[])
{
    struct ASTnode *n;

    if (argc != 4)
        usage(argv[0]);

    init();

    if ((inFile = fopen(argv[1], "r")) == NULL) {
        printf("Unable to open %s: %s\n", "inSourceCode", strerror(errno));
        return 1;
    }

    // Create the output file
    if ((outFile = fopen(argv[2], "w")) == NULL) {
        printf("Unable to create %s: %s\n", "outAssemblyCode", strerror(errno));
        return 1;
    }
    
    if ((wordFile = fopen(argv[3], "w")) == NULL) {
        printf("Unable to create %s: %s\n", "outWords", strerror(errno));
        return 1;
    }
    
    printf("Begin to Scan\n");

    int flag;
    struct ASTnode *tree;

    scan(&Token);			// Get the first token from the input
    
    printf("Begin to Generate\n");
    genPreamble();

    while (1) 
    {                   // Parse a function and
        tree = functionDeclaration(&flag);

        if(flag == SYNTAX_ERROR)
            return 1;
        printf("Begin to build AST\n");
        genAST(tree, NOREG, 0);     // generate the assembly code for it

        if (Token.token == T_EOF)   // Stop when we have reached EOF
            break;
    }

    genPostamble();

    printf("End Generation\n");
    fclose(outFile);
    fclose(wordFile);
    return (0);
}