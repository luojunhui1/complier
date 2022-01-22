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


// Parse the declaration of a variable
void varDeclaration(void) {

  // Ensure we have an 'int' token followed by an identifier
  // and a semicolon. Text now has the identifier's name.
  // Add it as a known identifier
  match(T_INT, "int");
  ident();
  addGlob(Text);
  genGlobSym(Text);
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
  nameslot= addGlob(Text);
  lparen();
  rparen();

  // Get the AST tree for the compound statement
  tree= compoundStatement(flag);

  // Return an A_FUNCTION node which has the function's nameslot
  // and the compound statement sub-tree
  return(mkastunary(A_FUNCTION, tree, nameslot));
}
