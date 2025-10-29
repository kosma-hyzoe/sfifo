#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
	char bufor[80];
	char* arg[10];
	int bg;

	while (1)
	{
		printf("msh $ ");
		fgets(bufor, 80, stdin);
		bg = AnalizujPolecenie(bufor, arg);

		if (arg[0] == NULL)
			continue;
		else if (strcmp(arg[0], "exit")==0)
			exit(0);
		else
			Wykonaj(arg, bg);
	}

}

int AnalizujPolecenie(char *bufor, char *arg[])
{
	int counter=0;
	int i=0, j=0;

	while (bufor[counter] != '\n')
	{
		while (bufor[counter] == ' ' || bufor[counter] == '\t')
                	counter++;
		if (bufor[counter] != '\n')
		{
			arg[i++] = &bufor[counter];
			while (bufor[counter] != ' ' && bufor[counter] != '\t' && bufor[counter] != '\n')
				counter++;
    	            if (bufor[counter] != '\n')
   	             	bufor[counter++] = '\0';
		}
	}
	bufor[counter] = '\0';
	arg[i]=NULL;
	if (i>0)
		while (arg[i-1][j] != '\0')
		{
			if (arg[i-1][j] == '&')
			{
				if (j == 0)
					arg[i-1] = NULL;
				else
					arg[i-1][j] = '\0';
				return 1;
			}
			j++;
		}
	return 0;
}


int Wykonaj(char **arg, int bg)
{
	pid_t pid;
	int status;

	if ((pid=fork()) == 0)
	{
		execvp(arg[0],arg);
		perror("Blad exec");
		exit(1);	}
	else if (pid > 0)
	{
		if (bg == 0)
			waitpid(pid, &status, 0);
		return 0;
	}
	else
	{
		perror("Blad fork");
		exit(2);
	}
}


