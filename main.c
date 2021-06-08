/*
 * @briefs: main process
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 21:07:22
 * @LastEditTime: 2021-06-08 16:38:45
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

static void usage(char *prog)
{
    fprintf(stderr, "Usage : %s infile\n", prog);
    exit(1);
}

char *tokstr[] = {"+", "-", "*", "/", "intlit"};


int main(int argc, char *argv[])
{
    struct ASTnode *n;

    if (argc != 4)
        usage(argv[0]);

    init();

    if ((inFile = fopen(argv[1], "r")) == NULL) {
        fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
        exit(1);
    }

    // Create the output file
    if ((outFile = fopen(argv[2], "w")) == NULL) {
        fprintf(stderr, "Unable to create %s: %s\n", argv[2], strerror(errno));
        exit(1);
    }
    
    if ((wordFile = fopen(argv[3], "w")) == NULL) {
        fprintf(stderr, "Unable to create %s: %s\n", argv[3], strerror(errno));
        exit(1);
    }
    
    scan(&Token);			// Get the first token from the input
    
    genPreamble();
    n = compoundStatement();
    genAST(n, NOREG, 0);	// Generate the assembly code for it
    genPostamble();

    fclose(outFile);
    fclose(wordFile);
    return (0);
}