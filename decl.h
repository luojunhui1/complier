/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:25:30
 * @LastEditTime: 2021-06-07 14:37:37
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

int genAST(struct ASTnode *n, int reg);
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
int cgEqual(int r1, int r2);
int cgNotEqual(int r1, int r2);
int cgLessThan(int r1, int r2);
int cgGreaterThan(int r1, int r2);
int cgLessEqual(int r1, int r2);
int cgGreaterEqual(int r1, int r2);


// misc.c

void match(int t, char *what);
void semi(void);
void ident(void);
void fatal(char *s);
void fatals(char *s1, char *s2);
void fatald(char *s, int d);
void fatalc(char *s, int c);

//stmt.c

void statements(void);

// sym.c

int findGlob(char *s);
int addGlob(char *name);

// decl.c

void varDeclaration(void);
