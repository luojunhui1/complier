/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:25:30
 * @LastEditTime: 2021-06-07 00:37:36
 */
int scan(struct token *t);

//tree.c

struct ASTnode *mkastnode(int op, struct ASTnode *left,
			  struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);

//expr.c

struct ASTnode *binexpr(int ptp);
void destoryAST(struct ASTnode *root);

// gen.c

int genAST(struct ASTnode *n);
void genPreamble();
void genPostamble();
void genFreeRegs();
void genPrintInt(int reg);
int interpretAST(struct ASTnode *n);
void generateCode(struct ASTnode *n);

//cg.c

void freeAllRegisters(void);
void cgPreamble();
void cgPostamble();
int cgLoad(int value);
int cgAdd(int r1, int r2);
int cgSub(int r1, int r2);
int cgMul(int r1, int r2);
int cgDiv(int r1, int r2);
void cgPrintInt(int r);

// misc.c

void match(int t, char *what);
void semi(void);

//stmt.c

void statements(void);