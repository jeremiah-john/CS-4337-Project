#ifndef LEXER_HEADER
#define LEXER_HEADER
void tokenize(FILE *fp);
void tokenINT(FILE *fp,int *firstdigit);
void tokenID(FILE *fp,int *firstchar);
void tokenString(FILE *fp);
int isSpecialWord();
#endif

