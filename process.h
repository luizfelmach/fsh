#ifndef PROCESS_H
#define PROCESS_H

#include <fcntl.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include "tokens.h"

typedef struct process Process;

Process *process_create(Tokens args, pid_t pgid, int bg);
int      process_completed(Process *p);
void     process_complete(Process *p, int status);
int      process_stopped(Process *p);
void     process_stop(Process *p);
void     process_continue(Process *p);
int      process_wait(Process *p);
void     process_setpgid(Process *p, pid_t pid);
pid_t    process_pid(Process *p);
Tokens   process_args(Process *p);
void     process_destroy(Process *p);

#endif  // !PROCESS_H
