#ifndef _SHELL_BUILTINS_H
#define _SHELL_BUILTINS_H
#include "readcmd.h"
    enum BUILTINS{
        B_ECHO,
        B_PWD, 
        B_WHO 
    };

    void builtin_handler();
    int is_builtin(struct cmdline *l);
#endif