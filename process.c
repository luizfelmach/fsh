#include "process.h"

struct process {
    Tokens args;
    pid_t  pid;
    int    completed;
    int    stopped;
    int    status;
};

void process_signals() {
    signal(SIGINT, SIG_IGN);
    signal(SIGQUIT, SIG_DFL);
    signal(SIGTSTP, SIG_DFL);
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    signal(SIGCHLD, SIG_DFL);
}

Process *process_create(Tokens args, pid_t pgid, int bg) {
    Process *p = malloc(sizeof(Process));

    pid_t pid = fork();

    p->pid       = pid;
    p->args      = args;
    p->completed = 0;
    p->stopped   = 0;
    p->status    = 0;

    if (pgid == 0) pgid = pid;

    if (pid > 0) {
        setpgid(pid, pgid);
        return p;
    }

    process_signals();
    setpgid(getpid(), pgid);

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (bg != 0) fork();

    execvp(p->args[0], p->args);
    printf("fsh: command not found: %s\n", p->args[0]);
    exit(1);
}

int process_completed(Process *p) {
    return p->completed;
}

void process_complete(Process *p, int status) {
    p->completed = 1;
    p->status    = status;
}

int process_stopped(Process *p) {
    return p->stopped;
}

void process_stop(Process *p) {
    p->stopped = 1;
}

void process_continue(Process *p) {
    p->stopped = 0;
}

int process_wait(Process *p) {
    int status;
    waitpid(p->pid, &status, WUNTRACED);
    if (WIFSTOPPED(status)) {
        kill(p->pid, WSTOPSIG(status));
        process_stop(p);
        return status;
    }
    process_complete(p, status);
    return status;
}

void process_setpgid(Process *p, pid_t pid) {
    setpgid(p->pid, pid);
}

pid_t process_pid(Process *p) {
    return p->pid;
}

Tokens process_args(Process *p) {
    return p->args;
}

void process_destroy(Process *p) {
    tokens_destroy(p->args);
    free(p);
}
