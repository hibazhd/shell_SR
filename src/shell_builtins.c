#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell_builtins.h"


int is_builtin(struct cmdline* l){
	return  (!strcmp(l->seq[0][0], "echo") || 
		 	 !strcmp(l->seq[0][0], "pwd") ||
		 	 !strcmp(l->seq[0][0], "whoami")
		 );
}

void builtin_handler(){}

