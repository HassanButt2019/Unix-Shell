//#include "lex.h"
#include <stdio.h>
#include "command.h"

void changebackground()
{
	int r;
	r = system("chngclur");
}
void backto()
{
	int r;
	r = system("backtonormal");
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
                for(j = i , k =1 ; home[j] != '\0' ; j++,k++)
                {
                        changehome[k] = home[j];
                }
                printf("%s@%s:%s>",user,host,changehome);

        }else
        printf("%s@%s:%s>",user,host,home);
}

//Executes all the commands in a single line
void executeCommand()
{
    char *temp[100]={NULL};
    char *temppipe[100]={NULL};

    // Checking if there are multiple commands seperated by ;
    parse( line , temppipe , ";" );
    int j=0;
    while(temppipe[j]!=NULL)
    {

        parse( temppipe[j] , temp , "|" );
        int i = 0;
        int fd[2];
        while( temp[i] != NULL )
        {
            char *tokens[100] = {NULL};
            char *temp1[100]={NULL};
            char *temp2[100]={NULL};

            checkOutfile(temp[i]);
            checkInfile(temp[i]);


            // Seperating commands and various arguments
            parse( temp[i] , temp1 , "<" );
            parse( temp1[0] , temp2 , ">" );
            parse( temp2[0] , tokens , " " );

            if(tokens[0]==NULL)
                return;

            int bg = backgroundCheck(tokens);
            signal(SIGCHLD,sig_handler);

            // Checking if command is builtin or not
            if ( strcmp(tokens[0],"quit") == 0)
            {   backto();
		 _exit(0);
	    }
            else if ( strcmp(tokens[0],"cd") == 0)
                func_CD(tokens);
            else if ( strcmp(tokens[0],"pwd") == 0)
                func_PWD();
            else if ( strcmp(tokens[0],"echo") == 0)
                func_ECHO(tokens);
/*            else if ( strcmp(tokens[0],"pinfo") == 0)
                executePINFO(tokens);
            else if ( strcmp(tokens[0],"jobs") == 0)
                executeJOBS();
            else if ( strcmp(tokens[0],"kjob") == 0)
                executeKJOB(tokens);
            else if ( strcmp(tokens[0],"overkill") == 0)
                executeOVERKILL();
            else if ( strcmp(tokens[0],"fg") == 0)
                executeFG(tokens); */
            else
            {
                pipe(fd);

                if(bg)
                    strcpy(job[JOBCTR].name,tokens[0]);

                pid_t  pid;
                int flag;

                pid = fork();
                if( pid < 0 )
                {
                    perror("Forking Error ");
                }
                else if(pid  == 0 )
                {

                    if(INFILE != 0)
                    {
                        dup2(INFILE,STDIN_FILENO);
                        close(INFILE);
                    }
                    if(OUTFILE != 1)
                    {
                        dup2(OUTFILE,STDOUT_FILENO);
                        close(OUTFILE);
                    }
                    if(temp[i+1]!=NULL) 
                    {
                        dup2(fd[1],STDOUT_FILENO);
                        close(fd[1]);
                    }

                    // executing command
                    if(execvp(*tokens,tokens) < 0)
                    {
                        perror("Error ");
                        exit(0);
                    }
                }
                else
                {
                    //Waiting for child process to end
                    if(!bg)
                        wait(&flag);
                    else
                    {
                        job[JOBCTR++].pid = pid;
                        printf("Process started: %s [%d]\n",tokens[0],pid);
                    }
                    INFILE=fd[0];
                    close(fd[1]);
                }
            }
            i++;
        }
        INFILE=0;
        OUTFILE=1;
        j++;
    }
}



int main(int argc , char *argv[])
{
	changebackground();
	SHELLPID = getpid();
	gethostname(host , 1000);
	getlogin_r(user , 1000);
	getcwd(execdir , 1000);
	
	while(1)
	{
		line[0] = '\0';
		signal(SIGINT , SIG_IGN);
		signal(SIGQUIT , SIG_IGN);
		signal(SIGTSTP , SIG_IGN);
		if(signal(SIGINT , sig_handler) == 0)
			continue;
		if(signal(SIGQUIT , sig_handler) == 0)
			continue;
		if(signal(SIGTSTP , sig_handler) == 0)
			continue;
		invokeShell();
		
		if(scanf("%[^\n]s",line) != EOF)
		{
			getchar();
			if(spaceCheck())
				continue;
		}else
		{
			putchar('\n');
			continue;
		}
		executeCommand();
	}

	return 0;
}
