#include "jobs.h"

int job_count = 0;
job_t jobs_table;
void create_job_table(){
    for (size_t i = 0; i < 256; i++){
        jobs_table[i] = 0;
    }
    
}

void create_new_job(pid_t pid_add){
    
    if (jobs_table[0]==0){
        job_count ++;
        jobs_table[0] = pid_add;
        printf("\nProcess PID : %d running... Job: [%d]\n", pid_add, job_count);
        return;
    }
    job_count ++;
    jobs_table[job_count-1] = pid_add;
    printf("\nProcess PID : %d running... Job: [%d]\n ", pid_add, job_count);
}

void terminate_job_by_number(int job_number){

    /*Reaping the child*/
    if(jobs_table[job_number-1]!=0){
        Waitpid(jobs_table[job_number-1], NULL, 0);
    }

    printf("\nJob [%d] : Done... PID : %d\n", job_number, jobs_table[job_number-1]);

    /*Handling memory reallocation*/
    
    if(job_number == job_count){
        jobs_table[job_count-1] = 0;
    }

    for (size_t i = job_number; i < job_count; i++){
        jobs_table[i] = jobs_table[i+1];

    }
    jobs_table[job_count-1]=0;
    job_count -- ;
}

void terminate_job_by_pid(pid_t term_pid){
     /*Handling memory reallocation*/

    int job_number=0;
    
    if(term_pid == jobs_table[job_count-1]){
        printf("\nJob [%d] : Done... PID : %d\nshell> ", job_count, jobs_table[job_count-1]);
        jobs_table[job_count-1] = 0;
        job_count --;
        return;
    }
    while (jobs_table[job_number]!=term_pid && job_number!=job_count){
        job_number++;
    }
    if(job_number == job_count)
        return;
    
    printf("\nJob [%d] : Done... PID : %d\nshell> ", job_number, jobs_table[job_number+1]);

    for (size_t i = job_number; i < job_count; i++){
        jobs_table[i] = jobs_table[i+1];

    }
    jobs_table[job_count-1]=0;
    job_count -- ;
}

int exist_job_table(){
    return job_count!=0;
}

int get_job_count(){
    return job_count;
}


