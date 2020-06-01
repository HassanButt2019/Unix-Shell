#include <stdio.h>
#include "command.h"

void changebackground() //change background into gray colour
{
	int r;
	r = system("chngclur");
}
void backto() //change background into white colour
{
	int r;
	r = system("backtonormal");
}
void invokeShell() // it will use to invoke the shell
{
        char home[1000];
        getcwd(home , 1000); //it takes the  current directory
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
        if(flag == 0)
        {
                int j , k;
                for(j = i , k =1 ; home[j] != '\0' ; j++,k++)
                {
                        changehome[k] = home[j];
                }
                printf("%s@%s:%s>",user,host,changehome);
        }else
        printf("%s@%s:%s>",user,host,home);
} //prints the current directory

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

        parse( temppipe[j] , temp , "|" );//parsing pipes
        int i = 0;
        int fd[2];
        while( temp[i] != NULL )
        {
            char *tokens[100] = {NULL};
            char *temp1[100]={NULL};
            char *temp2[100]={NULL};

            checkOutfile(temp[i]);//file open for output
            checkInfile(temp[i]); //file open for input

            // Seperating commands and various arguments
            parse( temp[i] , temp1 , "<" );
            parse( temp1[0] , temp2 , ">" );
            parse( temp2[0] , tokens , " " );

            if(tokens[0]==NULL)
                return;

            int bg = backgroundCheck(tokens);
            signal(SIGCHLD,sig_handler);/*A trap signal that indicates a process started by the current process has terminated.
This is how the shell knows to wait for a command to terminate before prompting for another command*/

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
                pipe(fd);//fd[0] will be the fd(file descriptor) for the 
//read end of pipe. //combines two command

                if( bg )//checks the command running in background or no
                    strcpy(job[JOBCTR].name,tokens[0]);//copies the current background job if not running

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
                        dup2(INFILE,STDIN_FILENO); //makes the duplicates of on file descriptor , making them alies and  then delete the old file descriptor
                        close(INFILE);//stdin_fileno  it he standard input file descriptor
                    }
                    if(OUTFILE != 1)
                    {
                        dup2(OUTFILE,STDOUT_FILENO); //stdout_fileno is the standard output file 
                        close(OUTFILE);
                    }
                    if(temp[i+1]!=NULL) 
                    {
                        dup2(fd[1],STDOUT_FILENO);
                        close(fd[1]);
                    }

                    // executing command
                    if(execvp(*tokens,tokens) < 0) // execvp is use to execute any process file 
                    {
                        perror("Error ");
                        exit(0);
                    }
                }
                else
                {
                    //Waiting for child process to end
                    if(!bg)
                        wait(&flag);//if job is runing than wait 
                    else
                    {
                        job[JOBCTR++].pid = pid;//current job pid
                        printf("Process started: %s [%d]\n",tokens[0],pid);
                    }
                    INFILE=fd[0]; // more than one readers can enter into a single file
                    close(fd[1]);//but not more than one writer can enter into a single file
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
	SHELLPID = getpid();//get the pid
	gethostname(host , 1000);
	getlogin_r(user , 1000);
	getcwd(execdir , 1000); //takes the current directory
	
	while(1)
	{
		line[0] = '\0';
		signal(SIGINT , SIG_IGN);
		signal(SIGQUIT , SIG_IGN);
		signal(SIGTSTP , SIG_IGN);
/* ctrl + c */		if(signal(SIGINT , sig_handler) == 0)//terminate the process
			continue;
/* ctrl + / */		if(signal(SIGQUIT , sig_handler) == 0)//terminate the process
			continue;
/* ctrl + z */		if(signal(SIGTSTP , sig_handler) == 0)//can continue the process
			continue;
		invokeShell();//invoking the shell

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
