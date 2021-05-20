#ifndef PARSER_HEADER
#define PARSER_HEADER
void program();
void stmt_list();
void stmt();
void printfunc();
void inputfunc();
void assign();
void ifstmt();
void whileloop();
void forloop();
void p_arg();
int expr();
int i_expr();
int n_expr();
int b_expr(int val);
int term();
int t_expr(int val);
int factor();
int f_expr(int val);
int value();
int v_expr(int val);
void runElseStmt();
void runThenStmt();
#endif
