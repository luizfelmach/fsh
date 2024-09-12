#ifndef SHELL_H
#define SHELL_H

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "job.h"
#include "list.h"
#include "process.h"
#include "tokens.h"

typedef struct shell Shell;

void shell_create();
void shell_prompt();
void shell_attach_fg();
void shell_send_process_to_fg(Process *p);
void shell_add_background(Process *p);
void shell_add_job(Job *j);
void shell_wait(Process *p);

void shell_wait_all();
void shell_die();

int is_internal_op(char *command);
int shell_internal_op(char *command);

void shell_signals();
void shell_destroy();

#endif
