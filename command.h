#include "lex.h"
/* Description :
	Start by creating a new process for each command in the pipeline and making
the parent wait for the last command. this will allow running simple commands


*/
//Execute All the commands in the single line

typedef struct jobs
{
    char name[100];
    pid_t pid;
}jobs;

jobs job[1000];

void backtonormal()
{
	int r;
	r = system("backtonormal");
}
void deleteJob(int jobpid)
{
	int i ;
	int flag = 0;
	for(i =1 ; i < JOBCTR ; i++)
	{
		if(job[i].pid == jobpid)
			flag = 1;
		if(flag = 1)
			job[i] = job[i+1];
	}
}

void sig_handler(int sig)
{
    if(sig == SIGINT)
    {
        printf("\n");
        invokeShell();
        fflush(stdout);
    }
    if(sig == SIGTSTP)
    {
        printf("\n");
        invokeShell();
        fflush(stdout);
    }
    if(sig == SIGQUIT)
    {
        printf("\n");
        invokeShell();
        fflush(stdout);
    }
    if(sig == SIGCHLD)
    {
 	int  wstat;
        pid_t pid;

        while(1)
        {
            pid = wait3(&wstat,WNOHANG,(struct rusage *)NULL);
            if(pid == 0 )
                return;
            else if(pid == -1)
                return;
            else
            {
                fprintf(stderr,"\nProcess with PID : %d exited with return value: %d\n",pid,wstat);
                deleteJob(pid);
            }
        }
    }
}
void invokeShell()
{
	char home[1000];
	getcwd(home , 1000);
	int flag = 0;
	char changehome[1000]="~";
	int i;
	if(strlen(execdir) <= strlen(home))
	{
		for(i =0 ; execdir[i] != '\0' ; i++)
		{
			if(execdir[i] != home[i])
			{
				flag = 1;
				break;
			}
		}
	}else
		flag = 1;
	if(flag = 0)
	{
		int j , k;
		for(j = i , k =1 ; home[j] != "\0" ; j++,k++)
		{
			changehome[k] = home[j];
		}
		printf("%s@%s:%s>",user,host,changehome);
	
	}else
	printf("%s@%s:%s>",user,host,home);
}
//void executeFG(char **tokens);

//void executeOVERKILL()
//{
//}
//void executeKJOB(char **tokens)
//{
//}
//void executeJOBS()
//{
//}


//void func_PINFO(char **tokens)
//{
//}

int backgroundCheck(char **tokens)
{
	int i =0;
 	while(tokens[i] != NULL)
	{
		if(strcmp(tokens[i] , "&")==0)
		{
			tokens[i] = NULL;
			return 1;
		}
		i++;
	}
	return 0;
}	

//Builtin Command
void func_PWD()
{
	char home[1000];
	getcwd(home,1000);
	printf("%s\n",home);
}
// builtin cd command

void func_CD(char **tokens)
{
	char home[1000];
	strcpy(home , execdir);
	int i ;
	int len = strlen(execdir);
	if (tokens[1] == NULL)
		chdir(execdir);
	else if(tokens[1][0] == "~")
	{
		for (i = 1 ; tokens[1][i] != "\0" ; i++)
		{
			home[i+len-1] = tokens[1][i];
		}
		home[i+len-1] = "\0";
		if(chdir(home) != 0)
		{
			perror("Error");
		} 
	}else if(chdir(tokens[1] != 0)) 
	{
		perror("Error");
	}
}
// Making built in echo command
void func_ECHO(char **tokens)
{
	int i = 1;
	int j = 0;
	char input[100000];

	int flag1 = 0;
	int flag2 = 0;
	
	while(tokens[i] != NULL)
	{
		int k ;
		for(k=0 ; tokens[i][k] != "\0" ; k++)
		{
			if(flag1 == 1)
			{
				if(tokens[i][k] == '\"')
					flag1= 0;
				else
					input[j++] = tokens[i][k];
			}else if (flag2==1)
			{
				if(tokens[i][k] == '\'')
				{
				flag2 = 0;	
				}else
				{
				input[j++] = tokens[i][k];
				}
			}else
			{
				if(tokens[i][k] == '\"')
					flag1=1;
				else if (tokens[i][k] == '\'')
					flag2 = 1;
				else
					input[j++] = tokens[i][k];
			}
		}
		i++;
	}
	input[j] = '\0';
	if(flag1 == 0 && flag2 == 0)
		printf("%s\n",input);
	else
		printf("Error: Wrong Input\n");
}
