#include <stdio.h>
#include "csapp.h"
#include "background.h"

void SIGCHILD_handler(int sig){
      while(waitpid(-1,NULL,WNOHANG)>0){
            sleep(0.5);
      };
}

void background_execute(struct cmdline* l){
      int pid_child;
      Signal(SIGCHLD,SIGCHILD_handler);
	if((pid_child= Fork())==0){ /*child */

            int nb_pipes = get_number_of_pipes(l->seq);
		if(nb_pipes>1){
			pipe_n_instructions(l, nb_pipes);
		} else
		/* Handling Builtins */
            if(!builtin_process(l)){/*executing Builtins */
			external_process(l);  /* executing externals */
		}
            exit(0);
      }
      else{
            return;
      }
}
