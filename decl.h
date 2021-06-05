/*
 * @Descriptions: 
 * @Author: Junhui Luo
 * @Blog: https://luojunhui1.github.io/
 * @Date: 2021-05-24 02:25:30
 * @LastEditTime: 2021-06-05 14:11:33
 */
int scan(struct token *t);

struct ASTnode *mkastnode(int op, struct ASTnode *left,
			  struct ASTnode *right, int intvalue);
struct ASTnode *mkastleaf(int op, int intvalue);
struct ASTnode *mkastunary(int op, struct ASTnode *left, int intvalue);
struct ASTnode *binexpr(int ptp);
void destoryAST(struct ASTnode *root);

int interpretAST(struct ASTnode *n);