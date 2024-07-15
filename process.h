#ifndef PROCESS_H
#define PROCESS_H

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include "tokens.h"

typedef struct process Process;

Process *process_create(Tokens args);
void     process_wait(Process *p);
void     process_setpgid(Process *p, pid_t pid);
pid_t    process_pid(Process *p);
Tokens   process_args(Process *p);
void     process_destroy(Process *p);

#endif  // !PROCESS_H
