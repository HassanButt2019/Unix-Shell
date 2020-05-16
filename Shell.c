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
typedef struct _instr instruction;
void func_clear()
{
	const char* blank = "\e[1;1H\e[2J";
	write(STDOUT_FILENO,blank,12);
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

int main(int argc,char *argv[] )
{
	signal(SIGINT , StopSignal);
	int i ;
	int pipe1 = pipe(fd);
	func_clear();
	func_pwd(cwd,0);
	
}	
