/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "shell_builtins.h"

int external_handler(struct cmdline *l){
	int pid_child;
	if((pid_child= Fork())==0){ /*child */
		
		int res = execvp(l->seq[0][0],l->seq[0]);
		if(res < 0){
			fprintf(stderr,"exec failed\n");
			exit(1);
		}
	}
	else{ /*pere*/
		Waitpid(pid_child, NULL, 0);
	}
	return 0;
}

int main(){
	while (1) {
		struct cmdline *l;

		printf("shell> ");
		l = readcmd();

		/*quitting*/
		if (!strcmp(l->seq[0][0],"quit")){
			printf("%s\n",l->seq[0][0]);
			printf("Exiting...\n");
			exit(0);
		}
		
		
		if(is_builtin(l)){/* Handling Builtins */
			builtin_handler(l);/* executing Builtins */
		}
		else{
			external_handler(l); /* executing externals */
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


