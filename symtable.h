#ifndef SYMTABLE_HEADER
#define SYMTABLE_HEADER

struct symbol
{
    char ident[32];
    char value[50];
};

void makeNewSymbol(char *id, char *userVal);
int isValInt(struct symbol *sym);
struct symbol * peekAtSymbol(char *id);
#endif
