#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

/*commands:
strdup = function returns a pointer to a new string which is
		 a duplicate of the string

strcat = function will append a copy of the source string to the end of destination string

execv = execute a command, return nothing if succedded, -1 if failed or error

strcpy = copy a string

strtok = cut a string by specific sign

*/

void read_cmd(char cmd[], char* par[])
{
	int count=0, i=0, j=0;
	char* array[100], *pch, str[100];
	gets(str);//read the command from the user
	count=strlen(str);
	if(count==0){strcpy(str,"\n");} // in case of enter
	if(count==1){return;}
	pch=strtok(str," ");
	while(pch!=NULL)
	{
		array[i++]=strdup(pch);
		pch=strtok(NULL," ");
	}
	strcpy(cmd,array[0]);
	for(int j=0;j<i;j++)
	{
		par[j]=array[j];
	}
	par[i]=NULL;
}


void exec_cmd(char *tok,char cmd[],char *args[])
{
	//execute the command with the token + '/' + arguments
	strcat(tok,"/");
	strcat(tok,cmd);
	execv(tok,args);
}


void split_into_array(char *arr[], int *n)
{
	//split the PATH into array of paths
	char* buf =getenv("PATH");
	int i = 0;
	char *p = strtok(buf, ":");
	char *array[20];
	while (p != NULL)
	{
		array[i++] = p;
		p = strtok (NULL, ":");
	}
	for(int j=0;j<i;j++)
	{
		arr[j]=array[j];
	}
	arr[i]=NULL;
	*n=i-1;
}


void main()
{
	int pid,stat,n,i=0;
	char token[1000];
	char *path;
	char cmd[1000],*params[100],*arr[100],**ptr;
	printf("MyShell>");
	read_cmd(cmd,params); //read the command from the user
	split_into_array(arr,&n); //split the PATH
	ptr=arr;// pointer to the path array
	if(!ptr){puts("Empty");}
	while(strcmp(cmd,"leave")!=0)
	{
	//while leave is not the input command
	//check if 'ENTER_KEY' was entered
	//otherwise - try to execute the command
		if(strcmp(cmd,"\n")==0)
		{
			printf("MyShell>");
		}
		else
		{
			pid=fork();
			while (*ptr)
			{
				strcpy(token,*ptr);
				if(pid == 0)//Child
				{//trying to execute the command
					exec_cmd(token,cmd,params);
				}
				else
				{
					wait(&stat);
					if(stat!=0){puts("command not found..");}
					break;
				}
				(*ptr)++;
				
			}
			ptr=arr;
			printf("MyShell>");
		}
		read_cmd(cmd,params); //enter another command
	}
}



