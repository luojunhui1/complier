/*
 * @briefs: main process
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 21:07:22
 * @LastEditTime: 2021-06-05 14:12:15
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

static void scan_file()
{
    struct token t;

    while(scan(&t))
    {
        printf("Token %s", tokstr[t.token]);

        if(t.token == T_INTLIT)
        {
            printf(", Val = %d", t.intval);
        }

        printf("\n");
    }
}

int main(int argc, char *argv[])
{
   struct ASTnode *n;

  if (argc != 2)
    usage(argv[0]);

  init();

  if ((inFile = fopen(argv[1], "r")) == NULL) {
    fprintf(stderr, "Unable to open %s: %s\n", argv[1], strerror(errno));
    exit(1);
  }

  scan(&Token);			// Get the first token from the input
  n = binexpr(0);		// Parse the expression in the file
  printf("%d\n", interpretAST(n));	// Calculate the final result
  return (0);
}