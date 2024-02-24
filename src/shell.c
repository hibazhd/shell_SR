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
#include "background.h"
#include "jobs.h"
// void displaycmd(struct cmdline *l ) {
// 	for (int i=0; l->seq[i]!=0; i++) {
// 		char **cmd = l->seq[i];
// 		printf("seq[%d]: ", i);
// 		for (int j=0; cmd[j]!=0; j++) {
// 			printf("%s ", cmd[j]);
// 		}
// 		printf("\n");
// 	}

// }



int main(){
	while (1) {
		struct cmdline *l;
		

		l = readcmd();
		
		if(l->err){
			fprintf(stderr,"%s\n",l->err);
			continue;
		}
		/*quitting*/
		if(!strcmp(l->seq[0][0],"quit") ||
			!strcmp(l->seq[0][0],"exit")){
			printf("Exiting...\n");
			exit(0);
		}	

		/*checking if command is meant to be ran in background*/

		if(l->bg){
		 	create_new_job(background_execute(l));
			continue;
		}


		if (!strcmp(l->seq[0][0],"fg")){
			if(exist_job_table()){
				if(l->seq[0][1] == NULL){
					terminate_job_by_number(get_job_count());
				}
				else{
					terminate_job_by_number(atoi(l->seq[0][1]));
				}
			}
			else{
				printf("No current jobs running\n");
			}

			continue;
		}
		
			/*Récupération du nombres de tubes*/
		int nb_pipes = get_number_of_pipes(l->seq);
		if(nb_pipes>1){
			pipe_n_instructions(l, nb_pipes);
			continue;
		} 


		/*Utilisation de continue pour éviter de rajouter des clauses else 
		et faire moins de nesting, pour auguementer la lisibilité*/
		
		
		/* Handling Builtins */
		if(!builtin_process(l)){/*executing Builtins */
			external_process(l);  /* executing externals */
		}
			
			
		
	}
}


