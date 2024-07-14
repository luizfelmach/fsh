#include "shell.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

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
    shell_ignore_signals();
}

void shell_prompt() {
    printf("fsh> ");
}

void shell_execute_fg(Tokens args) {
    Process* p = process_fg(args);
    process_spawn(p);
    process_wait(p);
}

void shell_execute_bg(Tokens args) {
    Process* p = process_bg(args);
    process_spawn(p);
}

void shell_destroy() {
    free(SHELL);
    shell_restore_signals();
}
