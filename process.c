#include "process.h"

struct process {
    Tokens args;
    pid_t  pid;
};

Process *process_create(Tokens args, int bg) {
    Process *p = malloc(sizeof(Process));

    pid_t pid = fork();

    p->pid  = pid;
    p->args = args;

    if (pid > 0) {
        setpgid(pid, pid);
        return p;
    }

    if (pid < 0) {
        perror("fork");
        exit(1);
    }

    if (bg != 0) fork();

    setpgid(getpid(), getpid());

    execvp(p->args[0], p->args);
    printf("fsh: command not found: %s\n", p->args[0]);
    exit(1);
}

void process_wait(Process *p) {
    int status;
    waitpid(p->pid, &status, 0);
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
