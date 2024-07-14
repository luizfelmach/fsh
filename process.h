#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>
#include <unistd.h>

typedef struct process Process;

Process *process_create(char **args, bool foreground);
Process *process_fg(char **args);
Process *process_bg(char **args);
void     process_spawn(Process *p);
void     process_wait(Process *p);
void     process_destroy(Process *p);

#endif  // !PROCESS_H
