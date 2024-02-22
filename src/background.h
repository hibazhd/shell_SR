#ifndef _BACKGROUND_H_
#define _BACKGROUND_H_
#include "csapp.h"
#include "readcmd.h"
#include "shell_builtins.h"
#include "piping.h"
#include "external_commands.h"
#include <stdint.h>
void background_execute(struct cmdline* l);
#endif