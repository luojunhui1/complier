/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-06-07 00:27:54
 * @LastEditTime: 2021-06-11 15:01:38
 */
#include "defs.h"
#include "data.h"
#include "decl.h"

struct ASTnode *singleStatement(int *flag);

/**
 * @brief match print statement
 * @param none
 * @return an AST root node with operation type --- print
 * @details: none
 */
static struct ASTnode *printStatement(int *flag) {
  struct ASTnode *tree;
  int reg;

  // Match a 'print' as the first token
  match(T_PRINT, "print");

  // Parse the following expression and generate the assembly code
  tree = binexpr(0, flag);
  
  // Make an print AST tree
  tree = mkastunary(A_PRINT, tree, 0);
  
  return (tree);
}

/**
 * @brief match assignment statement
 * @param none
 * @return an AST root node with operation type --- assign
 * @details when scaner get an indentifier, call this function to ensure that this variable has already been declared and add
 * this assignment statement into AST as the right node
 */
static struct ASTnode *assignmentStatement(int *flag) {
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
  left = binexpr(0, flag);

  // Make an assignment AST tree
  tree = mkastnode(A_ASSIGN, left, NULL, right, 0);

  return tree;
}


/**
 * @brief Parse an IF statement including any optional ELSE clause and return its AST
 * @param none
 * @return an AST root node with operation type --- if
 * @details: 
 */
struct ASTnode *ifStatement(int *flag) {
  struct ASTnode *condAST, *trueAST, *falseAST = NULL;

  // Ensure we have 'if' '('
  match(T_IF, "if");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0, flag);

  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  trueAST = compoundStatement(flag);

  // If we have an 'else', skip it
  // and get the AST for the compound statement
  if (Token.token == T_ELSE) {
    scan(&Token);
    falseAST = compoundStatement(flag);
  }
  // Build and return the AST for this statement
  return (mkastnode(A_IF, condAST, trueAST, falseAST, 0));
}

/**
 * @brief Parse a WHILE statement
 * @param flag syntax flag 
 * @return an AST root node with operation type --- while
 * @details while_statement: 'while' '(' true_false_expression ')' compound_statement  ;
 */
struct ASTnode *whileStatement(int *flag) {
  struct ASTnode *condAST, *bodyAST;

  // Ensure we have 'while' '('
  match(T_WHILE, "while");
  lparen();

  // Parse the following expression
  // and the ')' following. Ensure
  // the tree's operation is a comparison.
  condAST = binexpr(0, flag);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  rparen();

  // Get the AST for the compound statement
  bodyAST = compoundStatement(flag);

  // Build and return the AST for this statement
  return (mkastnode(A_WHILE, condAST, NULL, bodyAST, 0));
}
/**
 * @brief for statement
 * @param flag syntax flag
 * @return struct ASTnode* 
 * @details none
 */
struct ASTnode *forStatement(int* flag) {
  struct ASTnode *condAST, *bodyAST;
  struct ASTnode *preopAST, *postopAST;
  struct ASTnode *tree;

  // Ensure we have 'for' '('
  match(T_FOR, "for");
  lparen();

  // Get the pre_op statement and the ';'
  preopAST = singleStatement(flag);
  semi();

  // Get the condition and the ';'
  condAST = binexpr(0, flag);
  if (condAST->op < A_EQ || condAST->op > A_GE)
    fatal("Bad comparison operator");
  semi();

  // Get the post_op statement and the ')'
  postopAST = singleStatement(flag);
  rparen();

  // Get the compound statement which is the body
  bodyAST = compoundStatement(flag);

  // For now, all four sub-trees have to be non-NULL.
  // Later on, we'll change the semantics for when some are missing

  // Glue the compound statement and the postop tree
  tree = mkastnode(A_GLUE, bodyAST, NULL, postopAST, 0);

  // Make a WHILE loop with the condition and this new body
  tree = mkastnode(A_WHILE, condAST, NULL, tree, 0);

  // And glue the preop tree to the A_WHILE tree
  return (mkastnode(A_GLUE, preopAST, NULL, tree, 0));
}

/**
 * @brief Parse a single statement
 * @param flag syntax flag
 * @return struct ASTnode*
 * @details none 
 */
struct ASTnode *singleStatement(int *flag) 
{
  switch (Token.token) {
    case T_PRINT:
      return (printStatement(flag));
    case T_INT:
      varDeclaration();
      return (NULL);		// No AST generated here
    case T_IDENT:
      return (assignmentStatement(flag));
    case T_IF:
      return (ifStatement(flag));
    case T_WHILE:
      return (whileStatement(flag));
    case T_FOR:
      return (forStatement(flag));
    default:
      fatald("Syntax error, token", Token.token);
  }
}


/**
 * @brief Parse one or more statements
 * @param none
 * @return an AST root node end with right brace
 * @details: 
 */
struct ASTnode *compoundStatement(int *flag) {
  struct ASTnode *left = NULL;
  struct ASTnode *tree;

  // Require a left curly bracket
  lbrace();

  *flag = SYNTAX_CORRECT;
  
  while (1) {
    // Parse a single statement
    tree = singleStatement(flag);

    // Some statements must be followed by a semicolon
    if (tree != NULL && (tree->op == A_PRINT || tree->op == A_ASSIGN))
      semi();
    
    // For each new tree, either save it in left
    // if left is empty, or glue the left and the
    // new tree together
    if (tree != NULL) 
    {
      if (left == NULL)
      	left = tree;
      else
      	left = mkastnode(A_GLUE, left, NULL, tree, 0);
    }

    // When we hit a right curly bracket,
    // skip past it and return the AST
    if (Token.token == T_RBRACE) {
      rbrace();
      return (left);
    }

    if(*flag == SYNTAX_ERROR)
        return left;
  }
}