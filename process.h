#ifndef PROCESS_H
#define PROCESS_H

#include <unistd.h>

#include "command.h"

typedef struct process Process;

Process *process_create(Command *cmd);
pid_t process_spawn(Process *p);
Process *process_set_fg(Process *p);

#endif  // !PROCESS_H
