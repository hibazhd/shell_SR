#ifndef _JOBS_H_
#define _JOBS_H_
#include "csapp.h"
#include "readcmd.h"
#include <stdio.h>
typedef pid_t job_t[256]; 

void create_job_table();
void terminate_job_by_number(int job_number);
void create_new_job(pid_t pid_add);
void terminate_job_by_pid(pid_t term_pid);
int exist_job_table();
int get_job_count();
#endif