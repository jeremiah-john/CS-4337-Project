#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
//#include "lexer.h"
char currToken[510]; //I believe 509 is the maximum string size in C. we will need this to keep any temporary tokens we are lex-ing(?) through
FILE *output;
int isSpecialWord()
{
	/* this checks the currToken char array to see if it contains a special token like for, while, or ,etc.
	 * if it finds that it is a special token, it prints out the token (i.e. FOR_LOOP_TOKEN for "for") and returns 0
	 * otherwise, it returns -1 to show that it is not in the list of special tokens
	 */

     if(strcmp(currToken,"print") == 0)
     {
        fprintf(output,"PRINT_FUNC\n");
        return 0;
     }
     if(strcmp(currToken,"if") == 0)
     {
        fprintf(output,"IF_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"then") == 0)
     {
        fprintf(output,"IF_THEN_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"else") == 0)
     {
        fprintf(output,"IF_ELSE_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"get") == 0)
     {
        fprintf(output,"INPUT_FUNC\n");
        return 0;
     }
     if(strcmp(currToken,"end") == 0)
     {
        fprintf(output,"END_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"while") == 0)
     {
        fprintf(output,"WHILE_LOOP_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"do") == 0)
     {
        fprintf(output,"LOOP_DO_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"for") == 0)
     {
        fprintf(output,"FOR_LOOP_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"and") == 0)
     {
        fprintf(output,"AND_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"or") == 0)
     {
        fprintf(output,"OR_TOKEN\n");
        return 0;
     }
     if(strcmp(currToken,"not") == 0)
     {
        fprintf(output,"NOT_TOKEN\n");
        return 0;
     }
     return -1;
}
void tokenString(FILE *fp)
{
    memset(currToken,0,sizeof currToken);//this clears the currToken array of any remaining characters from a previous operation
    fscanf(fp, "%[^\"]",&currToken[0]);//we extract the actual string between the quotations (remember we scanned in the opening quotations, and now we
                                    //should be at the closing quotation, we need to advance past so as to not read another string again
    //printing out the token and the value of that token
    fprintf(output,"STRING(\"%s\")\n",currToken);
    fgetc(fp);//we just scan in the last quotation mark and "throw it away" since we don't want to read in another string
}
void tokenID(FILE *fp,int *firstchar) //I wanted to keep any parameters I pass as pointers since C doesn't do pass by reference, this should save memory
{
    memset(currToken,0,sizeof currToken);
    currToken[0] = *firstchar;
    fscanf(fp,"%[_a-zA-Z0-9]*",&currToken[1]);
    if(isSpecialWord() == -1)
    {
        fprintf(output,"ID(%s)\n",currToken);
    }
    /* the code above first attaches the first character of the ID we already scanned in to the token for analyzation. then we capture the rest of
     * the ID, which must only contain letters, underscores, or digits
     * we then need to check if the token is a special word, like "for" or "while", if not
     * then we print out the token and the name of the ID and exit this function
     */
}
void tokenINT(FILE *fp,int *firstdigit)
{
    memset(currToken,0,sizeof currToken);
    currToken[0] = *firstdigit;
    fscanf(fp,"%[0-9]*",&currToken[1]);
    fprintf(output,"INT(%s)\n",currToken);
}
void tokenize(FILE *fp)
{
    int c = fgetc(fp);
    output = fopen("lexoutput","wb");
    while (c != EOF)
    {
        if(c == '<')
	{
            c = fgetc(fp);
            if (c == '=')
	    {
		    fprintf(output,"LESSOREQUAL\n");
	    }	    
	    else 
	    {
	            fprintf(output,"LESSTHAN\n");
	            fseek(fp,-1,SEEK_CUR);
	    }
	}
	else if (c == '>')
	{
            c = fgetc(fp);
            if (c == '=')
	    {
		    fprintf(output,"GREATEROREQUAL\n");
	    }	    
	    else
	    {
	            fprintf(output,"GREATERTHAN\n");
	            fseek(fp,-1,SEEK_CUR);
	    }
	}
	else if(c == ';')
	{
	    fprintf(output,"SEMICOLON\n");
	}
	else if(c == '(')
	{
	    fprintf(output,"PARENOPEN\n");
	}
	else if(c == ')')
	{
            fprintf(output,"PARENCLOSE\n");
	}
	else if(c == '"')
	{
	    tokenString(fp);
	}
	else if(isalpha(c) || c == '_')
	{
            tokenID(fp,&c);
	}
	else if(isdigit(c))
	{
            tokenINT(fp,&c);
	}
	else if(c == '%')
	{
            fprintf(output,"MOD_OP\n");
	}
	else if(c == '*')
	{
            fprintf(output,"MULT_OP\n");
	}
	else if(c == '/')
	{
            fprintf(output,"DIV_OP\n");
	}
	else if(c == '=')
	{
            c = fgetc(fp);
            if (c == '=')
	    {
		    fprintf(output,"EQUAL_TO\n");
	    }	    
	    else
	    {
	            fprintf(output,"ASSIGN_OP\n");
	            fseek(fp,-1,SEEK_CUR);
	    }

	}
	else if(c == '+')
	{
            c = fgetc(fp);
            if (c == '+')
	    {
		    fprintf(output,"INCREMENT\n");
	    }	    
	    else
	    {
	            fprintf(output,"ADD_OP\n");
	            fseek(fp,-1,SEEK_CUR);
	    }

	}
	else if(c == '-')
	{
            c = fgetc(fp);
            if (c == '-')
	    {
		    fprintf(output,"DECREMENT\n");
	    }	    
	    else
	    {
	            fprintf(output,"SUB_OP\n");
	            fseek(fp,-1,SEEK_CUR);
	    }

	}
	else if(c == '!')
	{
            c = fgetc(fp);
            if (c == '=')
	    {
		    fprintf(output,"NOT_EQUAL_TO\n");
	    }	    
	    else
	    {
	            fprintf(output,"ERROR\n");
	            fseek(fp,-1,SEEK_CUR);
	    }
	}
	c = fgetc(fp);
    }
    fprintf(output,"ENDSTREAM\n");
    fclose(output);
}


