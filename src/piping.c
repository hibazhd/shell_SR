#include "csapp.h"
#include "piping.h"

void write_in_pipe(int fd[2]){
    Close(fd[0]);
    Close(STDOUT_FILENO); // Close standard output
    Dup2(fd[1], STDOUT_FILENO); // Redirect stdout to the writing end of the pipe
    Close(fd[1]); // Close the writing end of the pipe
}

void read_from_pipe(int fd[2]){
    Close(fd[1]); // Close writing end of the pipe
    Close(STDIN_FILENO); // Close standard input
    Dup2(fd[0], STDIN_FILENO); // Redirect stdin to the reading end of the pipe
    Close(fd[0]); // Close the reading end of the pipe
}

void pipe_n_instructions(struct cmdline* l, int nb_comms){
    
    int fd[2];
    pipe(fd);
    int nb_comms_execute = 0;
    if ((Fork())==0){
        nb_comms_execute++;
        if(l->in){
			int f_in = Open(l->in,O_RDONLY,S_IRUSR|S_IWUSR);
			Dup2(f_in, 0);
		}

        write_in_pipe(fd);
		if(execvp(l->seq[0][0],l->seq[0]) < 0){
			fprintf(stderr,"Unknown Command\n");
			exit(1);
		}
    }

    else{

        while(nb_comms_execute!=nb_comms-1){
            
        }

        if ((Fork())==0){

            if(l->out){
			    int f_out = Open(l->out, O_CREAT|O_RDWR, S_IRUSR|S_IWUSR);
			    Dup2(f_out, 1);
		    }

            read_from_pipe(fd);
            
            if(execvp(l->seq[1][0],l->seq[1]) < 0){
                fprintf(stderr,"Unknown Command\n");
                exit(1);
            }
        }
        Close(fd[1]); // Close reading end of the pipe in the parent
        Close(fd[0]); // Close writing end of the pipe in the parent
        Wait(NULL); // Wait for both child processes to finish
        Wait(NULL);
    }
}