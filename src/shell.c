/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "shell_builtins.h"

int main()
{
	while (1) {
		struct cmdline *l;
		int i, j;

		printf("shell> ");
		l = readcmd();

		if (l->err) {
			/* Syntax error, read another command */
			printf("error: %s\n", l->err);
			continue;
		}

		if (l->in) printf("in: %s\n", l->in);
		if (l->out) printf("out: %s\n", l->out);

		/* Display each command of the pipe */
		displaycmd(l);

		/*quitting*/
		if (!strcmp(l->seq[0][0],"quit")){
			printf("%s\n",l->seq[0][0]);
			printf("Exiting...\n");
			exit(0);
		}
		
		/* Handling Builtins */
		if(is_builtin(l)){
			builtin_handler(l);
		}
			
		
	}
}

void displaycmd(struct cmdline *l ){
	for (int i=0; l->seq[i]!=0; i++) {
		char **cmd = l->seq[i];
		printf("seq[%d]: ", i);
		for (int j=0; cmd[j]!=0; j++) {
			printf("%s ", cmd[j]);
		}
		printf("\n");
	}

}

int is_builtin(struct cmdline* l){
	return  (!strcmp(l->seq[0][0], "echo") || 
		 	 !strcmp(l->seq[0][0], "pwd") ||
		 	 !strcmp(l->seq[0][0], "whoami")
		 );
}