/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:25:30
 * @LastEditTime: 2021-06-11 14:28:18
 */

int scan(struct token *t);
int analysis();

//tree.c

struct ASTnode *mkastnode(int op, struct ASTnode *left, struct ASTnode *mid,
			  struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);

//expr.c

struct ASTnode *binexpr(int ptp, int *flag);
void destoryAST(struct ASTnode *root);

// gen.c

int genAST(struct ASTnode *n, int reg, int parentASTop);
void genPreamble();
void genPostamble();
void genFreeRegs();
void genPrintInt(int reg);
int interpretAST(struct ASTnode *n);
void genGlobSym(char *s);

//cg.c

void freeAllRegisters(void);
void cgPreamble();
void cgPostamble();
int cgLoadInt(int value);
int cgAdd(int r1, int r2);
int cgSub(int r1, int r2);
int cgMul(int r1, int r2);
int cgDiv(int r1, int r2);
void cgPrintInt(int r);
int cgStorGlob(int r, char *identifier);
void cgGlobSym(char *sym);
int cgLoadGlob(char *identifier);

int cgCompareAndSet(int ASTop, int r1, int r2);
int cgCompareAndJump(int ASTop, int r1, int r2, int label);
void cgLabel(int l);
void cgJump(int l);


// misc.c

void match(int t, char *what);
void semi(void);
void lbrace(void);
void rbrace(void);
void lparen(void);
void rparen(void);
void ident(void);
int fatal(char *s);
int fatals(char *s1, char *s2);
int fatald(char *s, int d);
int fatalc(char *s, int c);

//stmt.c

struct ASTnode *compoundStatement(int *flag);

// sym.c

int findGlob(char *s);
int addGlob(char *name);

// decl.c

void varDeclaration(void);
