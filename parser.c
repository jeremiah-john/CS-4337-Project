#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"
#include "symtable.h"
char currSymbol[100];
FILE *lexercode;
int linecount;
void getNextToken()
{
    memset(currSymbol,0,sizeof currSymbol); //reset currSymbol so it doesn't carry over old values from previous writes
    char *buff = currSymbol;
    size_t bufferSize = 100;
    
    fgets(buff, bufferSize, lexercode);
    //on Windows, lines end with \r\n, not \n. so we need to do a little function here to cut that out;
   /* while(buff = strchr(currSymbol,'\r'))
    {
        *buff = ' ';
    }*/
}
int getIntTokenValue()
{
    char numVal[50];
    for(int i = 4; i < strlen(currSymbol); i++)
    {
        if(currSymbol[i] ==')')
        {
            break;
        }
        numVal[i-4] = currSymbol[i];
    }
    return atoi(numVal);
}
void goBackAToken() //I guess this is backtracking? we tend to use this whenever a b_expr or v_expr or whatever turns out to be empty
//in that case the next token we get belongs to a different nonterminal than the one we're parsing through, so we must go back;
{
     fseek(lexercode,0-strlen(currSymbol),SEEK_CUR);
}
int t_expr(int val)
{
    if(strcmp("ADD_OP\n",currSymbol) == 0)
    {
        getNextToken();
        return (val + n_expr());
    }
    if(strcmp("SUB_OP\n",currSymbol) == 0)
    {
        getNextToken();
        return (val - n_expr());
    }
    goBackAToken();
   return val;
}
int f_expr(int val)
{
   if(strcmp("MULT_OP\n",currSymbol) == 0)
   {
       getNextToken();
       return (val * term());
   }
   if(strcmp("DIV_OP\n",currSymbol) == 0)
   {
       getNextToken();
       return (val / term());
   }
   if(strcmp("MOD_OP\n",currSymbol) == 0)
   {
       getNextToken();
       return (val % term());
   }
   goBackAToken();
   return val;
}
int v_expr(int val)
{
   if(strcmp("GREATERTHAN\n",currSymbol) == 0)
   {
       getNextToken();
       return (val > value());
   }
   if(strcmp("GREATEROREQUAL\n",currSymbol) == 0)
   {
       getNextToken();
       return (val >= value());
   }
   if(strcmp("LESSTHAN\n",currSymbol) == 0)
   {
       getNextToken();
       return (val < value());
   }
   if(strcmp("LESSOREQUAL\n",currSymbol) == 0)
   {
       getNextToken();
       return (val <= value());
   }
   if(strcmp("EQUAL_TO\n",currSymbol) == 0)
   {
       getNextToken();
       return (val == value());
   }
   if(strcmp("NOT_EQUAL_TO\n",currSymbol) == 0)
   {
       getNextToken();
       return (val != value());
   }
    goBackAToken();
   return val;
}
int value()
{
   int returnVal;
   if(strcmp(currSymbol,"PARENOPEN\n") == 0)
   {
       getNextToken();
       returnVal = expr();
       getNextToken();
       if(strcmp(currSymbol,"PARENCLOSE\n") != 0)
       {
           printf("Missing closing parenthesis on line %d",linecount);
           exit(-1);
       }
   }
   else if(strcmp(currSymbol,"NOT_TOKEN\n") == 0)
   {
       getNextToken();
       returnVal = 0 == value() ? 1 : 0;
   }
   else if(strcmp(currSymbol,"SUB_OP\n") == 0) //if we have "- <value>" that actually means to return a negative of that value.
   {
       getNextToken();
       returnVal = 0 - value();
   }
   else if(strncmp("ID",currSymbol,2) == 0)
   {
       char *idStr = strtok(currSymbol, "(");
       idStr = strtok(NULL,")"); 
       struct symbol *variable = peekAtSymbol(idStr);
       if(!variable) //a null pointer would be 0, and !0 would be true, so we are checking if peekAtSymbol couldn't find our variable
       {
           printf("FATAL ERROR at line %d: could not find ID %s \n terminating program...\n",linecount,idStr);
           exit(-1);
       }
       if(isValInt(variable))
       {
           returnVal = atoi(variable->value);
       }
       else
       {
           printf("FATAL ERROR at line %d: the ID %s contains a String Variable and thus cannot be used in math computations\n terminating program...\n",linecount,idStr);
           exit(-1);
       }
   }
   else if(strncmp("INT",currSymbol,3) == 0)
   {
       returnVal = getIntTokenValue();
   }
   return returnVal;
}
int factor()
{
   int valueVal = value();
   getNextToken();
   return v_expr(valueVal);
}
int term()
{
    int factVal = factor();
    getNextToken();
    int f_exp_Val = f_expr(factVal);
}
int b_expr(int val)
{
   if(strcmp("AND_TOKEN\n",currSymbol) == 0)
   {
       getNextToken();
       return (val && n_expr());
   }
   if(strcmp("OR_TOKEN\n",currSymbol) == 0)
   {
       getNextToken();
       return (val || n_expr());
   }
   goBackAToken();
   return val;
}
int n_expr()
{
   int termVal = term();
   getNextToken();
   return t_expr(termVal);
}
int expr()
{
   int n_result = n_expr();
   getNextToken();
   return b_expr(n_result);
}
void removeDoubleSlash(char *str)
{
    char *tmp;
    while(tmp = strchr(str,'\\'))
    {
        if(*(tmp+1) == 'n')
        {
           *tmp = '\n';
           *(tmp+1) = ' '; 
        }
        else if(*(tmp+1) == 't')
        {
            *tmp = '\t';
            *(tmp+1) = ' ';
        }
    }
}
void p_arg()
{
    if(strncmp("STRING",currSymbol,6) == 0)
    {
        char *actString = strtok(currSymbol, "\""); 
        actString = strtok(NULL,"\""); //these two pieces of code above basically go thru the string token and extracts the contents between the quotations
        removeDoubleSlash(actString);
        printf("%s",actString);
        /*observation: I was worried about having to free() actString since it seems dynamically allocated, but in reality it just holds one pointer to a character in currSymbol which is statically allocated.*/
    }
    else if(strncmp("ID",currSymbol,2) == 0)
    {
        char *idStr = strtok(currSymbol, "(");
        idStr = strtok(NULL,")"); 
        struct symbol *variable = peekAtSymbol(idStr);
        printf("%s\n",variable->value);
    }
    else
    {
       int i = expr(); //expr() returns an int even if we are expecting a string; how to fix this?
       printf("%d\n",i);
    }
}
void printfunc()
{
    getNextToken();
    p_arg();
}
void inputfunc()
{
    getNextToken();
    if(strncmp("ID",currSymbol,2) == 0)
    {
       char *idStr = strtok(currSymbol, "(");
       idStr = strtok(NULL,")"); //now ID should hold the actual id name
       char *userInput = malloc(50 * sizeof(char));
       scanf("%s",userInput);
       makeNewSymbol(idStr,userInput);
       free(userInput);
    }
    else
    {
        printf("ERROR at line %d: usage of get is \"get ID\". you need to supply a variable to store the user input!\n", linecount);
        exit(-1);
    }
}
void ifstmt()
{
    getNextToken();
    if(expr())
    {
        getNextToken();
        if(strcmp("IF_THEN_TOKEN\n",currSymbol) == 0)
        {
            runThenStmt();
        }
    }
    else
    {
        runElseStmt();
    }
}
void runThenStmt()
{
     getNextToken();
     stmt_list();
     while(strcmp("END_TOKEN\n",currSymbol) != 0) //while we have not reached the end of the if statement
     {
         getNextToken();
     }
}
void runElseStmt()
{
     while(strcmp("IF_ELSE_TOKEN\n",currSymbol) != 0)
     {
         getNextToken(); //we skip over the statments in the "then" protion of the if statement
     }
     getNextToken();
     stmt_list();
}
void assign()
{
     char *idStr = strtok(currSymbol, "(");
     idStr = strtok(NULL,")");
     char ident[32];
     memcpy(ident,idStr,32); //being careless with pointers includes having a pointer to changing data( which might cause some illegal memory access
     struct symbol *ID = peekAtSymbol(ident);
     getNextToken();
     if(strcmp("ASSIGN_OP\n",currSymbol) != 0)
     {
         printf("ERROR at line %d: expected '=' since current context dicates that you are trying to assign a value to a variable like so: ID = <expr>", linecount);
         exit(-1);
     }
     getNextToken();
     if(ID == 0) //if ID does not exist
     {
         char value[50];
         if(strncmp("STRING",currSymbol,6) == 0)
         {
            char *actString = strtok(currSymbol, "\""); //copy pasted from p-arg, I know
            actString = strtok(NULL,"\"");
            strcpy(value,actString);
         }
         else
         {
             sprintf(value,"%d",expr());
         }
         makeNewSymbol(ident,value);
     }
     else 
     {
         if(strncmp("STRING",currSymbol,6) == 0)
         {
            char *actString = strtok(currSymbol, "\""); //copy pasted from p-arg, I know
            actString = strtok(NULL,"\"");
            sprintf(ID->value, "%s",actString); 
         }
         else
         {
             sprintf(ID->value, "%d",expr()); 
         }
     }  
}
void stmt()
{
    if(strcmp("PRINT_FUNC\n",currSymbol) == 0)
    {
      printfunc();
    }
    else if(strcmp("INPUT_FUNC\n",currSymbol) == 0)
    {
      inputfunc();
    }
    else if(strcmp("IF_TOKEN\n",currSymbol) == 0)
    {
       ifstmt();
    }
    else if(strncmp("ID",currSymbol,2) == 0) //ID tokens also contain variable names which can,well,vary. so we just check the beginning to see if it is an ID
    {
       assign();
    }
    if(strcmp("WHILE_LOOP_TOKEN",currSymbol) == 0)
    {
       //whileloop();
    }
    
}
void stmt_list()
{
     
     if(strcmp("IF_ELSE_TOKEN\n",currSymbol) == 0 || strcmp("END_TOKEN\n",currSymbol) == 0)
     {
         return;
     }
     if(strcmp("ENDSTREAM\n",currSymbol) != 0) //if there is actual code in the program
     {
         stmt();
         getNextToken();
         if(strcmp("SEMICOLON\n",currSymbol) == 0)
         {
             linecount++;
             getNextToken();
             stmt_list();
         }
         else
         {
             printf("Missing Semicolon on line %d !\n",linecount);
             exit(-1);
         }
     } //otherwise it won't do anything
}
void program()
{
        linecount = 1;
	lexercode = fopen("./lexoutput","rb+");
	getNextToken();
	stmt_list();
	
}
