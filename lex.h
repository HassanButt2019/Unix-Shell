#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
/* lex.h : simple lexical analyzer for the UNIX-SHELL */

/* Global Variable */
char host[1000];
char user[1000];
char homedir[1010]="/home/";
char line[1000006];
char execpath[1000];
char execdir[1000];

int INFILE = 0;
int OUTFILE = 1;
int JOBCTR = 1;
int PIPEIN = 0;
pid_t SHELLPID;


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
		if(line[i] != " " && line[i] != "\t")
			return 0;
		i++;
	}
	return 1;
}

void checkInfile(char *tokens)
{
	int i = 0;
	char *temp[100] = {NULL};
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
		OUTFILE = open(temp[0] , O_TRUNC | O_WRONLY | O_CREAT , S_IRWXU);
	}else
	{
		OUTFILE = 1;
	}
}
