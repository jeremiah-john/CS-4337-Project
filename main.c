#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

int main(int argc, char *argv[])
{
	FILE *input;

	input = fopen(argv[1],"rb");	

	tokenize(input);

	program();
	return 0;
}
