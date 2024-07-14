#include "process.h"

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#include "shell.h"

struct process {
    char **args;
    pid_t  pid;
    bool   running;
    bool   foreground;
};

Process *process_create(char **args, bool foreground) {
    Process *p = malloc(sizeof(Process));

    p->args       = args;
    p->running    = false;
    p->pid        = 0;
    p->foreground = foreground;

    return p;
}

Process *process_fg(char **args) {
    Process *p = process_create(args, true);
    return p;
}

Process *process_bg(char **args) {
    Process *p = process_create(args, false);
    return p;
}

void process_destroy(Process *p) {
    free(p);
}

void process_spawn(Process *p) {
    pid_t pid = fork();
    p->pid    = pid;

    if (pid > 0) return;

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (p->foreground) {
        shell_restore_signals();
    } else {
        setsid();
        fork();
    }

    /* If error do not close the shell */
    execvp(p->args[0], p->args);
    printf("fsh: command not found: %s\n", p->args[0]);
    exit(1);
}

void process_wait(Process *p) {
    int status;
    waitpid(p->pid, &status, 0);
}
