#ifndef _SHELL_BUILTINS_H
#define _SHELL_BUILTINS_H
#include "readcmd.h"
    enum BUILTINS{
        B_INTERNAL,
        B_ECHO,
        B_PWD, 
        B_WHO 
    };

    int builtin_process(struct cmdline* l);
    int get_number_of_pipes(char*** seq);
#endif