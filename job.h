#ifndef JOB_H
#define JOB_H

#include <signal.h>
#include <string.h>

#include "process.h"

typedef struct job Job;

Job     *job_init(char *command);
Process *job_spawn(Job *j);
Process *job_contains(Job *j, pid_t pid);
Process *job_foreground(Job *j);
Process *job_background(Job *j, int pos);
int      job_background_size(Job *j);
int      job_active(Job *j);
void     job_signal(Job *j, int sig);
void     job_destroy(Job *j);

#endif