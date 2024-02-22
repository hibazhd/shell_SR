#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "csapp.h"
#include "shell_builtins.h"

int get_number_of_pipes(char*** seq){
	int nb_pipes=0;
	while(seq[nb_pipes]!=NULL){
		nb_pipes++;
	}
	return nb_pipes;
}

enum BUILTINS analyse_command(char **l){
    if (!strcmp(l[0],"echo")){
        return B_ECHO;
    }
    else if(!strcmp(l[0],"pwd")){
        return B_PWD;
    }
    else if(!strcmp(l[0],"whoami")){
        return B_WHO;
    }
    return B_INTERNAL;
}

int builtin_process(struct cmdline* l){
    enum BUILTINS command = analyse_command(l->seq[0]);
    int out_flag = 0;
    int f_out;

            /*Handling Redirections*/
    if(l->out){
        f_out = Open(l->out,O_RDWR|O_CREAT,S_IRUSR|S_IWUSR);
        out_flag = 1;
    }

    
    switch (command)
    {
    case B_ECHO:
            /*In case of redirection*/
        if(out_flag){
            if(Write(f_out, l->seq[0][1],strlen(l->seq[0][1])) != strlen(l->seq[0][1])){
                unix_error("Write Error");
            }
            Close(f_out);
            return 1;
        }

        printf("%s\n", l->seq[0][1]);
        return 1;
        break;
    case B_PWD:
        char buf[256];
        getcwd(buf, 256);

            /*In case of redirection*/
        if(out_flag){
            if(Write(f_out, buf,strlen(buf)) != strlen(buf)){
                unix_error("Write Error");
            }
            Close(f_out);
            return 1;
        }

        printf("%s\n",buf);
        return 1;
        break;
    case B_WHO:
        char *log_buf = getlogin();


            /*In case of redirection*/
        if(out_flag){
            if(Write(f_out, log_buf,strlen(log_buf)) != strlen(log_buf)){
                unix_error("Write Error");
            }
            Close(f_out);
            return 1;
        }

        printf("%s\n",log_buf);
        return 1;
        break;

    default:
        /* Not a builtin */
        return 0;
        break;
    }
}

