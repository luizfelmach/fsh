#include "shell.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "process.h"

struct shell {};

static Shell* SHELL;

void sigint_handler(int signum) {
    /*
     * TODO: Verificar se tem algum processo rodando. Se tiver,
     * sera necessário perguntar, senão é só dar exit mesmo.
     */
    printf("\nDo you really want to exit ? [y/n] ");
    char resp;
    scanf("%c", &resp);
    if (resp == 'y') {
        shell_destroy();
        exit(0);
    }
}

void shell_ignore_signals() {
    signal(SIGINT, sigint_handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGCHLD, SIG_IGN);
}

void shell_restore_signals() {
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
}

void shell_create() {
    SHELL = calloc(1, sizeof(Shell));
}

void shell_prompt() {
    printf("fsh> ");
}

void shell_attach_fg() {
    tcsetpgrp(STDOUT_FILENO, getpid());
}

void shell_send_process_to_fg(Process* p) {
    tcsetpgrp(STDOUT_FILENO, process_pid(p));
}

void shell_destroy() {
    free(SHELL);
    shell_restore_signals();
}
