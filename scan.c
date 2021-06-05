/*
 * @briefs: Lexical Scanner
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:26:42
 * @LastEditTime: 2021-06-04 09:20:13
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "decl.h"
#include "data.h"

/**
 * @brief find the postion of c in string s
 * @param s     string
 * @param c     character to be found
 * @return      the index of c in strinf s
**/
static int chrpos(char *s, int c) {
  char *p;

  p = strchr(s, c);
  return (p ? p - s : -1);
}

/**
 * @brief find next charater in file which is pointed by inFile
 * @param      none
 * @return     next character in file 
**/
static int next()
{
    int c;

    if(Putback)
    {
        c = Putback;
        Putback = 0;
        return c;
    }

    c = fgetc(inFile);
    if(c == '\n')
        Line++;
    
    return c;
}

/**
 * @brief putback unwanted character back
 * @param      c unwanted character
 * @return     none
**/
static void putback(int c)
{
    Putback = c;
}

/**
 * @brief skip blank space, newline and tabs 
 * @param       none
 * @return      next vaild character
**/ 
static int skip()
{
    int c;

    c = next();

    while(c == '\t' || c == ' ' || c == '\n' || c == '\f' || c == '\f')
        c = next();

    return c;    
}

/**
 * @brief scan int value
 * @param c     character to be scanned
 * @return      integer value 
 **/
static int scanint(int c)
{
    int k,val = 0;

    while((k = chrpos("0123456789", c)) >= 0)
    {
        val = val * 10 + k;
        c = next();
    }

    putback(c);

    return val;
}

int scan(struct token *t)
{
    int c;

    c = skip();

    switch (c)
    {
    case EOF:
       t->token = T_EOF;
       return 0;
    case '+':
        t->token = T_CROSS;
        break;
    case '-':
        t->token = T_HORIZONTAL;
        break;
    case '*':
        t->token = T_ASTERISK;
        break;
    case '/':
        t->token = T_OBLIQUE;
        break;
    default:
        if(isdigit(c))
        {
            t->token = T_INTLIT;

            t->intval = scanint(c);

            break;
        }
        
        printf("Unrecognized character %c on line %d\n", c, Line);
        exit(1);
    }

    return 1;
}