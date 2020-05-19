#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcnt1.h>
#include <signal.h>
/* lex.h : simple lexical analyzer for the UNIX-SHELL */

//used to tokenize the input lines into commands

void parse( char *line , char **tokens , char* delim)
{
	char *temp;
	temp = strtok(line , delim); //tokenize the line on the basis of delimeter;
	int i = 0;
	while(temp != NULL)
	{
		tokens[i++] = temp;
		temp = strtok(NULL , delim);
	}
}

int spaceCheck()
{
	int i =0;
	while(line[i] != "\0")
	{
		if(line[i] != '' && line[i] != "\t")
			return 0;
		i++;
	}
	return 1;
}

void checkInfile(char *tokens)
{
	int i = 0;
	char *tem[100] = {NULL};
	parse(tokens , temp , "<");
	if(temp[1] != NULL)
	{
		char *temp1[100]={NULL};
		parse(temp[1] , temp1 , " ");
		INFILE = open(temp1[0] ,  O_RDONLY); //It creates an open file description that refers to a file and a file descriptor that refers to that open file description
	}
}
void checkOutfile(char *tokens)
{
	int i = 0 ;
	char *temp[100] = {NULL};
	parse(tokens , temp , ">");
	if (temp[1] != NULL)
	{
		char*temp1[100]={NULL};
		parse(temp[1] , temp1 , " ");
		//different mode of files;
		OUTFILE = open(temp[0] , O_TRUNC | O_WRONLY | )_CREAT , S_IRWXU);
	}else
	{
		OUTFILE = 1;
	}
}
