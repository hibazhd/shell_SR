#include <stdio.h>
#include <stdlib.h>
#include "readcmd.h"
#include "csapp.h"
#include "external_commands.h"

void child_SIGINT_handler(int sig){
    Kill(getppid(),SIGCHLD);
	printf("\n");
	exit(1);
}
pid_t external_process(struct cmdline *l){
	int pid_child;
	if((pid_child= Fork())==0){ /*child */
		Signal(SIGINT, child_SIGINT_handler);
        /*Handling Redirections*/
		if(l->in){
			int f_in = Open(l->in,O_RDONLY,S_IRUSR|S_IWUSR);
			Dup2(f_in, 0);
		}

		if(l->out){
			int f_out = Open(l->out, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
			Dup2(f_out, 1);
		}


		
		if(execvp(l->seq[0][0],l->seq[0]) < 0){
			fprintf(stderr,"Unknown Command\n");
			exit(1);
		}
	}
	else{ /*pere*/
		Waitpid(pid_child, NULL, 0);
	}
	return pid_child;
}
