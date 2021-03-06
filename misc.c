/*
 * @Descriptions: lu
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-07 00:21:28
 * @LastEditTime: 2021-06-09 15:12:33
 */
#include <stdlib.h>

#include "defs.h"
#include "data.h"
#include "decl.h"

// Miscellaneous functions
// Copyright (c) 2019 Warren Toomey, GPL3

// Ensure that the current token is t,
// and fetch the next token. Otherwise
// throw an error 
void match(int t, char *what) {
  if (Token.token == t) {
    scan(&Token);
  } else {
    fatals("expected ", what);
  }
}

// Match a semicon and fetch the next token
void semi(void) {
  match(T_SEMI, ";");
}


// Match a left brace and fetch the next token
void lbrace(void) {
  match(T_LBRACE, "{");
}

// Match a right brace and fetch the next token
void rbrace(void) {
  match(T_RBRACE, "}");
}

// Match a left parenthesis and fetch the next token
void lparen(void) {
  match(T_LPAREN, "(");
}

// Match a right parenthesis and fetch the next token
void rparen(void) {
  match(T_RPAREN, ")");
}

// Match an identifer and fetch the next token
void ident(void) {
  match(T_IDENT, "identifier");
}

// Print out fatal messages
int fatal(char *s) {
  fprintf(stderr, "%s on line %d\n", s, Line);
  return(1);
}

int fatals(char *s1, char *s2) {
  fprintf(stderr, "%s:%s on line %d\n", s1, s2, Line); return (1);
}

int fatald(char *s, int d) {
  fprintf(stderr, "%s:%d on line %d\n", s, d, Line); return (1);
}

int fatalc(char *s, int c) {
  fprintf(stderr, "%s:%c on line %d\n", s, c, Line); return (1);
}
