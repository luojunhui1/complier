/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-07 00:27:54
 * @LastEditTime: 2021-06-08 16:01:53
 */
#include "defs.h"
#include "data.h"
#include "decl.h"

/**
 * @brief match print statement
 * @param none
 * @return an AST root node with operation type --- print
 * @details: none
 */
static struct ASTnode *printStatement(void) {
  struct ASTnode *tree;
  int reg;

  // Match a 'print' as the first token
  match(T_PRINT, "print");

  // Parse the following expression and generate the assembly code
  tree = binexpr(0);
  
  // Make an print AST tree
  tree = mkastunary(A_PRINT, tree, 0);
  
  // Match the following semicolon
  semi();

  return (tree);
}

/**
 * @brief match assignment statement
 * @param none
 * @return an AST root node with operation type --- assign
 * @details when scaner get an indentifier, call this function to ensure that this variable has already been declared and add
 * this assignment statement into AST as the right node
 */
static struct ASTnode *assignmentStatement(void) {
  struct ASTnode *left, *right, *tree;
  int id;

  // Ensure we have an identifier
  ident();

  // Check it's been defined then make a leaf node for it
  if ((id = findGlob(Text)) == -1) {
    fatals("Undeclared variable", Text);
  }
  right = mkastleaf(A_LVIDENT, id);

  // Ensure we have an equals sign
  match(T_ASSIGN, "=");

  // Parse the following expression
  left = binexpr(0);

  // Make an assignment AST tree
  tree = mkastnode(A_ASSIGN, left, NULL, right, 0);

  // Match the following semicolon
  semi();

  return tree;
}


/**
 * @brief Parse an IF statement including any optional ELSE clause and return its AST
 * @param none
 * @return an AST root node with operation type --- if
 * @details: 
 */
struct ASTnode *ifStatement(void) {
  struct ASTnode *condAST, *trueAST, *falseAST = NULL;

  // Ensure we have 'if' '('
  match(T_IF, "if");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);

  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  trueAST = compoundStatement();

  // If we have an 'else', skip it
  // and get the AST for the compound statement
  if (Token.token == T_ELSE) {
    scan(&Token);
    falseAST = compoundStatement();
  }
  // Build and return the AST for this statement
  return (mkastnode(A_IF, condAST, trueAST, falseAST, 0));
}

/**
 * @brief Parse a WHILE statement
 * @param none 
 * @return an AST root node with operation type --- while
 * @details while_statement: 'while' '(' true_false_expression ')' compound_statement  ;
 */
struct ASTnode *whileStatement(void) {
  struct ASTnode *condAST, *bodyAST;

  // Ensure we have 'while' '('
  match(T_WHILE, "while");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  bodyAST = compoundStatement();

  // Build and return the AST for this statement
  return (mkastnode(A_WHILE, condAST, NULL, bodyAST, 0));
}

/**
 * @brief Parse one or more statements
 * @param none
 * @return an AST root node end with right brace
 * @details: 
 */
struct ASTnode *compoundStatement(void) {
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  // Require a left curly bracket
  lbrace();

  while (1) {
    switch (Token.token) {
      case T_PRINT:
	      tree = printStatement();
	      break;
      case T_INT:
      	varDeclaration();
	      tree = NULL;		// No AST generated here
	      break;
      case T_IDENT:
        tree = assignmentStatement();
        break;
      case T_IF:
        tree = ifStatement();
        break;
      case T_WHILE:
        tree = whileStatement();
        break;
      case T_RBRACE:
        // When we hit a right curly bracket, skip past it and return the AST
        rbrace();
        return (left);
      default:
        fatald("Syntax error, token", Token.token);
    }

    // For each new tree, either save it in left
    // if left is empty, or glue the left and the
    // new tree together
    if (tree) {
      if (left == NULL)
	      left = tree;
      else
	      left = mkastnode(A_GLUE, left, NULL, tree, 0);
    }
  }
}
