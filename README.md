# Unix-Shell
A shell program is an application that allows interacting with the computer.In a shell the user can run programs and also redirect the input to come from a file and output to come from a file.Shells also provide programming constructions such as **if , for , while , functions , variables , etc.**. Additionally , shell programs offer features such as line editing , history , file complition , wild cards , enviroment variable expension , and programming construction.
## Parts Of a Shell Program
The shell implementation is divided into three parts: **The Parser , The Executor  , and   Shell Subsystems**.
## The Parser
 The Parser is the software component that reads the command line such as “ls ­al” and puts it into a data structure called ​Command Table​ that will store the commands that will be executed.  
## The Executor 
The executor will take the command table generated by the parser and for every SimpleCommand in the array it will create a new process. It will also if necessary create pipes to communicate the output of one process to the input of the next one. Additionally, it will redirect the standard input, standard output, and standard error if there are any redirections.
## Shell Subsystems
The subsystems that complete your shell are:
* Enviroment Variables:
* Wildcards
* Subshells
## Using Lex and Yacc to implement the Parser
A parser is divided into two parts: laxical Analyzer which takes the input characters and puts the characters together into words called tokes and a Parser which takes the processes of the tokens according to grammar and build the command table.
## The ( lex.h ) contains the implementation of the Lexical Analyzer
The lexical analyzer separates input into tokens.It will read the characters one by one from the standard input and form a token that will be passed to the parser.



























   
