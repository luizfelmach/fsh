#ifndef SHELL_H
#define SHELL_H

#include "process.h"
#include "tokens.h"

typedef struct shell Shell;

void shell_create();
void shell_prompt();
void shell_attach_fg();
void shell_send_process_to_fg(Process *p);
void shell_add_background(Process *p);

void shell_ignore_signals();
void shell_restore_signals();
void shell_destroy();

#endif  // !SHELL_H
