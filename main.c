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
    fprintf(stderr, "Usage : %s infile\n", prog);
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
    
    scan(&Token);			// Get the first token from the input
    
    printf("Begin to Generate\n");
    genPreamble();
    n = compoundStatement(&flag);

    if(flag == SYNTAX_ERROR)
        return 1;
    
    printf("Begin to build AST\n");
    genAST(n, NOREG, 0);	// Generate the assembly code for it
    genPostamble();

    printf("End Generation\n");
    fclose(outFile);
    fclose(wordFile);
    return (0);
}