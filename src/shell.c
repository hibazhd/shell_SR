/*
 * Copyright (C) 2002, Simon Nieuviarts
 */

#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "shell_builtins.h"
#include "external_commands.h"
#include "piping.h"

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

int get_number_of_pipes(char*** seq){
	int nb_pipes=0;
	while(seq[nb_pipes]!=NULL){
		nb_pipes++;
	}
	return nb_pipes;
}

int main(){
	while (1) {
		struct cmdline *l;

		printf("shell> ");
		l = readcmd();
		
			/*Récupération du nombres de tubes*/
		
		int nb_pipes = get_number_of_pipes(l->seq);
		if(nb_pipes>1){
			pipe_n_instructions(l, nb_pipes);
			continue;
		} 
		/*Utilisation de continue pour éviter de rajouter une clause else 
		et faire moins de nesting, pour auguementer la lisibilité*/
		
		
		/* Handling Builtins */
		if(!builtin_process(l)){/*executing Builtins */
			external_process(l);  /* executing externals */
		}
			
		
		
			/*quitting*/
		if (!strcmp(l->seq[0][0],"quit") ||
			!strcmp(l->seq[0][0],"exit")){
			printf("Exiting...\n");
			exit(0);
		}	
		
	}
}


