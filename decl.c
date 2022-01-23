/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-07 01:37:08
 * @LastEditTime: 2021-06-07 02:01:10
 */
#include "defs.h"
#include "data.h"
#include "decl.h"

// Parsing of declarations
// Copyright (c) 2019 Warren Toomey, GPL3

/**
 * @brief parse key words to primary types
 * @param t key word type
 * @return int primary type index
 */
int parseType(int t) {
  if (t == T_CHAR) return (P_CHAR);
  if (t == T_INT)  return (P_INT);
  if (t == T_VOID) return (P_VOID);
  fatald("Illegal type, token", t);
}

// Parse the declaration of a variable
void varDeclaration(void) {
  int id, type;

  type = parseType(Token.token);
  scan(&Token);
  ident();
  id = addGlob(Text, type, S_VARIABLE);
  genGlobSym(id);
  semi();
}

/**
 * @brief function decalration
 * @param none
 * @return struct ASTnode*
 * @details none 
 */
struct ASTnode *functionDeclaration(int *flag) {
  struct ASTnode *tree;
  int nameslot;

  // Find the 'void', the identifier, and the '(' ')'.
  // For now, do nothing with them

  match(T_VOID, "void");
  ident();
  nameslot= addGlob(Text, P_NONE, S_FUNCTION);
  lparen();
  rparen();

  // Get the AST tree for the compound statement
  tree= compoundStatement(flag);

  // Return an A_FUNCTION node which has the function's nameslot
  // and the compound statement sub-tree
  return(mkastunary(A_FUNCTION, P_VOID, tree, nameslot));
}
