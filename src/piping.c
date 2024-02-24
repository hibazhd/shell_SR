#include "csapp.h"
#include "piping.h"
#include "jobs.h"

void pipe_SIGINT_handler(int sig){
    Kill(getppid(),SIGCHLD);
    printf("\n");
    exit(1);
}

int get_number_of_pipes(char*** seq){
	int nb_pipes=0;
	while(seq[nb_pipes]!=NULL){
		nb_pipes++;
	}
	return nb_pipes;
}

void write_in_pipe(int fd[2]){
    close(fd[0]);
    close(STDOUT_FILENO); // Close standard output
    Dup2(fd[1], STDOUT_FILENO); // Redirect stdout to the writing end of the pipe
    close(fd[1]); // Close the writing end of the pipe
}

void read_from_pipe(int fd[2]){
    close(fd[1]); // Close writing end of the pipe
    close(STDIN_FILENO); // Close standard input
    Dup2(fd[0], STDIN_FILENO); // Redirect stdin to the reading end of the pipe
    close(fd[0]); // Close the reading end of the pipe
}

void pipe_n_instructions(struct cmdline* l, int nb_comms){
    
    int pipes[nb_comms - 1][2]; // Create an array of pipes for communication between commands

    // Create pipes
    for (int i = 0; i < nb_comms - 1; i++) {
        if (pipe(pipes[i]) < 0) {
            perror("Pipe creation failed");
            exit(1);
        }
    }

    // Fork and execute commands
    for (int i = 0; i < nb_comms; i++) { 
        pid_t child_pid;       
        if ((child_pid = Fork() == 0)) { // Child process
            
            
            Signal(SIGINT, pipe_SIGINT_handler);

            // Set up redirections
            if (i > 0) { // If not the first command
                read_from_pipe(pipes[i - 1]); // Redirect stdin from the previous pipe
            }
            if (i < nb_comms - 1) { // If not the last command
                write_in_pipe(pipes[i]); // Redirect stdout to the current pipe
            }

            // Handle input/output redirection
            if (l->in) {
                int f_in = Open(l->in, O_RDONLY, S_IRUSR | S_IWUSR);
                Dup2(f_in, 0);
            }
            if (l->out) {
                int f_out = Open(l->out, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
                Dup2(f_out, 1);
            }

            // Execute the command
            if (execvp(l->seq[i][0], l->seq[i]) < 0) {
                fprintf(stderr, "Unknown Command\n");
                exit(1);
            }
        } else { // Parent process
            
            

            // Close unnecessary pipe ends
            if (i > 0) {
                close(pipes[i - 1][0]); // Close reading end of the previous pipe
                close(pipes[i - 1][1]); // Close writing end of the previous pipe
            }
        }
    }

    // Close all pipe ends in the parent process
    for (int i = 0; i < nb_comms - 1; i++) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < nb_comms; i++) {
        Wait(NULL);
    }
}
