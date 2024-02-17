#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_builtins.h"


void builtin_process(struct cmdline* l){
    enum BUILTINS command = analyse_command(l->seq[0]);
    switch (command)
    {
    case B_ECHO:
    printf("%s", l->seq[0][1]);
        break;
    case B_PWD:
        break;
    case B_WHO:
        break;
    default:
        break;
    }
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
    
}