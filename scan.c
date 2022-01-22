/*
 * @briefs: Lexical Scanner
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:26:42
 * @LastEditTime: 2021-06-09 15:13:40
 */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "defs.h"
#include "decl.h"
#include "data.h"

static char *tokens[] = {"EOF", 
                  "+", "-", "*", "/",
                  "==", "!=",
                  "<", ">", "<=", ">=",
                  "Val", ";", "=", "Var",
                  "{", "}", "(", ")",
                  "print", "int", "if", "else", "while", "for", "void"
};

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

/**
 * @brief scan indentifier from input file and store it in buff[]
 * @param c first character caned from input file
 * @param buf pointer of input file or text
 * @param lim maxium of identifier length
 * @return the length of identifier
 * @details: 
 */
static int scanident(int c, char *buf, int lim)
{
    int i = 0;

    while(isdigit(c) || isalpha(c) || '_' == c)
    {
        if(lim - 1 == i)
        {
            fprintf(stderr, "identifier too long on line %d\n", Line);
            return (1);
        }
        else
        {
            buf[i++] = c;
        }
        c = next();
    }

    putback(c);
    buf[i] = '\0';

    return (i);
}

/**
 * @brief identify the keyword type of given word
 * @param s pointer to given word
 * @return the keyword type of given type
 * @details: 
 */
static int keyword(char *s) {
  switch (*s) {
    case 'e':
        if(!strcmp(s, "else"))
            return (T_ELSE);
        break;
    case 'i':
        if(!strcmp(s, "if"))
            return (T_IF);
        if (!strcmp(s, "int"))
            return (T_INT);
        break;
    case 'p':
        if (!strcmp(s, "print"))
            return (T_PRINT);
        break;
    case 'w':
        if(!strcmp(s, "while"))
            return (T_WHILE);
        break;
    case 'f':
        if(!strcmp(s, "for"))
            return (T_FOR);
        break;
    case 'v':
        if (!strcmp(s, "void"))
            return (T_VOID);
      break;
    
  }
  return (0);
}

/**
 * @brief get next token from input file
 * @param t pointer of token 
 * @return on getting token success, return 1, else if get the end of file return 0
 * @details: 
 */
int scan(struct token *t)
{
    int c,token_type;

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
    case ';':
        t->token = T_SEMI;
        break;
    case '{':
      t->token = T_LBRACE;
      break;
    case '}':
      t->token = T_RBRACE;
      break;
    case '(':
      t->token = T_LPAREN;
      break;
    case ')':
      t->token = T_RPAREN;
      break;
    case '=':
        if((c = next()) == '=')
            t->token = T_EQ;
        else
        {
            putback(c);
            t->token = T_ASSIGN;
        }
        break;
    case '!':
        if((c = next()) == '=')
            t->token = T_NE;
        else
            fatalc("Unrecognised character", c);
        break;
    case '<':
        if ((c = next()) == '=') {
            t->token = T_LE;
        } else {
            putback(c);
            t->token = T_LT;
        }
        break;
    case '>':
        if ((c = next()) == '=') {
            t->token = T_GE;
        } else {
            putback(c);
            t->token = T_GT;
        }
        break;
    default:
        if(isdigit(c))
        {
            t->token = T_INTLIT;

            t->intval = scanint(c);

            break;
        }else if(isalpha(c) || '_' == c)
        {
            scanident(c, Text, TEXTLEN);
            
            if(token_type = keyword(Text))
            {
                t->token = token_type;
            }
            else
                t->token = T_IDENT;
            break;
        }
        else
            fatalc("Unrecognised character", c);
    }

    if(t->token == T_INTLIT)
        fprintf(wordFile, "%s\t%d\t\n", tokens[t->token], t->intval);
    else if(t->token == T_IDENT)
        fprintf(wordFile, "%s\t%s\t\n", tokens[t->token], Text);
    else
        fprintf(wordFile, "%s\t%s\t\n", tokens[t->token], "-");

    return 1;
}