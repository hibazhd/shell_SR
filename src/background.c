#include <stdio.h>
#include "csapp.h"
#include "background.h"
#include "jobs.h"

void SIGCHILD_handler(int sig){
      int reaped_pid;
      while((reaped_pid = waitpid(-1,NULL,WNOHANG))>0){
            terminate_job_by_pid(reaped_pid);
            sleep(0.5);
      };
}

/*Fonctions auxiliaires ressemblant à celles déjà faites, mais sans le fork*/
void piping_instructions_background(struct cmdline* l, int nb_pipes){
      int pipes[nb_pipes - 1][2]; // Create an array of pipes for communication between commands

            // Create pipes
      for (int i = 0; i < nb_pipes - 1; i++) {
            if (pipe(pipes[i]) < 0) {
                  perror("Pipe creation failed");
                  exit(1);
            }
      }

      // Fork and execute commands
      for (int i = 0; i < nb_pipes; i++) {   
              
            if (Fork() == 0) { // Child process   

                  Signal(SIGINT, SIG_IGN);

                  // Set up redirections
                  if (i > 0) { // If not the first command
                  read_from_pipe(pipes[i - 1]); // Redirect stdin from the previous pipe
                  }
                  if (i < nb_pipes - 1) { // If not the last command
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
      for (int i = 0; i < nb_pipes - 1; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
      }
}

void execute_external_background(struct cmdline* l){
      /* executing externals */
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

pid_t background_execute(struct cmdline* l){
      int pid_child;
      Signal(SIGCHLD,SIGCHILD_handler);


      int nb_pipes = get_number_of_pipes(l->seq);
      if(nb_pipes>1){
            piping_instructions_background(l, nb_pipes);     
      }

	if((pid_child = Fork())==0){ /*child */
            Signal(SIGINT, SIG_IGN);
            if(!builtin_process(l)){/*executing Builtins */
                  execute_external_background(l);
            }
            exit(0);
      }
      else{
            return pid_child;
      }
}
