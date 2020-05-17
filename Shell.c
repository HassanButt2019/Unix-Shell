#include <stdlib.h>
#include <pwd.h>
#include <stdio.h>
#include <unistd.h> //it will provide to access of the POSIX operating system API
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/errno.h>//errno.h is a header file in the standard library of the C programming language. It defines macros for reporting and retrieving error conditions using the symbol errno. errno acts like an integer variable.
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
//#include <fcnt1.h> //fcntl. h is the header in the C POSIX library for the C programming language that contains constructs that refer to file control, e.g. opening a file, retrieving and changing the permissions of file, locking a file for edit,
#define DEF "\x1B[0m"
#define BUFSIZE 10000
#define INBUF 10000
#define ARGMAX 10

struct _instr
{
 char* argval[INBUF];
 int argcount;
};
char cwd[BUFSIZE];
int fd[2];
int filepid;
int exitflag = 0;
int exitfile = 0;
char *input , *input1;
char argval[ARGMAX];
int argcount = 0;
int inBackground = 0;
int externalIn=0 , externalOut = 0;
char inputfile[INBUF] , outputfile[INBUF];
typedef struct _instr instruction;
void func_clear()
{
	const char* blank = "\e[1;1H\e[2J";
	write(STDOUT_FILENO,blank,12);
}
//Fetch the screen
void screenfetch()
{
	func_clear();
	char *welcm = "\n                           ./+o+-\n                  yyyyy- -yyyyyy+\n               ://+//////-yyyyyyo\n           .++ .:/++++++/-.+sss/`\n         .:++o:  /++++++++/:--:/-\n        o:+o+:++.`..```.-/oo+++++/\n       .:+o:+o/.          `+sssoo+/\n  .++/+:+oo+o:`             /sssooo.\n /+++//+:`oo+o               /::--:.\n \\+/+o+++`o++o               ++////.\n  .++.o+++oo+:`             /dddhhh.\n       .+.o+oo:.          `oddhhhh+\n        \\+.++o+o``-````.:ohdhhhhh+\n         `:o+++ `ohhhhhhhhyo++os:\n           .o:`.syhhhhhhh/.oo++o`\n               /osyyyyyyo++ooo+++/\n                   ````` +oo+++o\\:    Unix-Shell\n                          `oo++.    Made by HASSAN BUTT\n\n";
	printf("%s",welcm);
}
void change_background()
{
	int r;
	r =system("chngclur");
}
void change_background_again()
{
        int r;
        r = system("backtonormal");
}
// get input containing spaces and tabs and store it in argval
void getInput()
{
	fflush(stdout);//clears the input buffer
	input = NULL;
	ssize_t buf = 0;
	getline(&input , &buf ,stdin);
	input1 = (char*) malloc(strlen(input) * sizeof(char));
	strncpy(input1,input , strlen(input));
	argcount = 0;
	inBackground = 0;
	while((argval[argcount] = strsep(&input," \t\n")) != NULL && argcount < ARGMAX - 1)
	{
	if(sizeof(argval[argcount-1]) == 0)
	{
		free(argval[argcount]);
	}else
	{
		argcount++;
	}
	if(strcmp(argval[argcount-1],"&")==0)
	{
		inBackground = 1;
		return;
	}
	}
	free(input);
}
void func_pwd(char * cwdstr , int command)
{
	char temp[BUFSIZE];
	char* path=getcwd(temp , sizeof(temp));

	if(path != NULL)
	{
	strcpy(cwdstr , temp);
	if(command == 1)//check if pwd is to be printed
	{
		printf("%s\n",cwdstr);
	}
	}else 
	{
	perror("+---ERROR IN getcwd() :");
	}
}
//My webscrap command which scraps songs from particular link
void web_scrap()
{
	int web;
	web = system("hay");
}

// stop processes if running in the terminal, close terminal if only CTRL+C
void StopSignal()
{
	if(filepid != 0)
	{
	int temp=filepid;
	kill(filepid,SIGINT);
	filepid=0;
	}
}

// use execvp to run the command , check path , and handle errors
void runprocess(char* cli , char* args[] , int count)
{
	int ret = execvp(cli , args);
	char* pathm;
	pathm = getenv("PATH");//check for all paths
	char path[1000];
	strcpy(path,pathm);
	strcat(path,":");
	strcat(path,cwd);
	char* cmd = strtok(path, ":\r\n");
	while(cmd != NULL)
	{
		char loc_sort[1000];
		strcpy(loc_sort,cmd);
		strcat(loc_sort,"/");
		strcat(loc_sort , cli);
		printf("execvp: %s\n",loc_sort);
		ret = execvp(loc_sort , args);
		if(ret == -1)
		{
			perror("Error in execution");
			exit(0);	
		}
		cmd = strtok(NULL,":\r\n");//tokenize the string with delimeter
	}
}
int main(int argc,char *argv[] )
{
	int back_arg = 0;
        change_background();
	signal(SIGINT , StopSignal);
	int i ;
	int pipe1 = pipe(fd);
	func_clear();
	func_pwd(cwd,0);
	screenfetch();
	while(exitflag == 0)
	{
		externalIn  = 0;
		externalOut = 0;
		inBackground = 0;
		printf("%s%s ~",DEF , cwd);
		getInput();
		if (strcmp(argval,"pwd") == 0 && !inBackground)
		{
			func_pwd(cwd,1);
		}else if (strcmp(argval,"hay")==0 && !inBackground)
		{
			web_scrap();
		}

		back_arg = 1;
	}
	
	if(back_arg == 1)
	change_background_again();
}	
