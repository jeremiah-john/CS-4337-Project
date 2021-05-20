#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "symtable.h"


//the value array will hold both strings and ints. whenever we would need to get the int value, we can use atoi() in addition to the code
//we used to check if the string only had integers in order to check if the value we stored was actually an int.
struct symbol symbolTable[64];
int symbolCount = 0;
void makeNewSymbol(char *id, char *userVal)
{
    strcpy(symbolTable[symbolCount].ident,id);
    strcpy(symbolTable[symbolCount].value,userVal);
    symbolCount++;
}
struct symbol * peekAtSymbol(char *id)
{
  //I know a simple for loop is not optimized but hey its the end of the semester and I'm tired
  for (int i = 0; i < 64; i++) //we use 64 because that is the length of SymbolTable[]
  {
      if(strcmp(symbolTable[i].ident,id) == 0) //if we found the variable with the specified id, return it as a pointer
      {
          return &symbolTable[i];
      }
  }
  return NULL; //if we can't find the specified symbol, then that means the data does not exist
}
int isValInt(struct symbol *sym) //returns 1 if the symbol in the table represents an integer variable. otherwise it returns 0
{
    for(int i = 0; i < 50; i++)
    {
        if(isalpha(sym->value[i]))
        {
            return 0;
        }
        if(sym->value[i] == '\0') //if we have reached the end of the value (and inherently haven't found an alpha char, then return 1
        {
            return 1;
        }
    }
    return 1;
}

