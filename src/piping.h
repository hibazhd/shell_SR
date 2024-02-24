#ifndef _PIPING_H_
#define _PIPING_H_
#include "csapp.h"
#include "readcmd.h"
#include <stdint.h>

int get_number_of_pipes(char*** seq);
void pipe_n_instructions(struct cmdline* l, int nb_comms);
void read_from_pipe(int fd[2]);
void write_in_pipe(int fd[2]);
#endif