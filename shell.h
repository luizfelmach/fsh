#ifndef SHELL_H
#define SHELL_H

#include "tokens.h"

typedef struct shell Shell;

void shell_create();
void shell_prompt();
void shell_execute_fg(Tokens args);
void shell_execute_bg(Tokens args);
void shell_ignore_signals();
void shell_restore_signals();
void shell_destroy();

#endif  // !SHELL_H
