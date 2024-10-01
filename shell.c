#define _POSIX_C_SOURCE 200809L

#include "shell.h"

static List* jobs;
static List* processes;

Process* shell_find_process(pid_t pid) {
    for (List* i = processes; i != NULL; i = list_next(i)) {
        if (process_pid(list_item(i)) == pid) return list_item(i);
    }

    return NULL;
}

Job* shell_find_job(pid_t pid) {
    for (List* i = jobs; i != NULL; i = list_next(i)) {
        if (job_contains(list_item(i), pid)) return list_item(i);
    }

    exit(0);

    return NULL;
}

int shell_active_jobs() {
    for (List* i = jobs; i != NULL; i = list_next(i)) {
        if (job_active(list_item(i))) return 1;
    }
    return 0;
}

Process* shell_child_change() {
    int   status;
    pid_t pid = waitpid(-1, &status, WNOHANG | WUNTRACED | WCONTINUED);

    if (pid <= 0) return NULL;

    Process* p = shell_find_process(pid);
    Job*     j = shell_find_job(pid);

    if (WIFEXITED(status)) {
        process_complete(p, status);
    }

    if (WIFSIGNALED(status)) {
        process_complete(p, status);
        job_signal(j, WTERMSIG(status));
    }

    if (WIFSTOPPED(status)) {
        process_stop(p);
        job_signal(j, WSTOPSIG(status));
    }

    if (WIFCONTINUED(status)) {
        process_continue(p);
    }

    return p;
}

void sigint_handler(int signum) {
    if (!shell_active_jobs()) exit(0);

    printf("\n");
    printf("You have an active job running!\n");
    printf("Would you like to exit now? (y/n) ");

    char resp;
    scanf("%c", &resp);
    if (resp == 'y') {
        shell_destroy();
        exit(0);
    }
}

void sigchld_handler(int signum) {
    while (shell_child_change() != NULL);
}

void sigtstp_handler(int signum) {
    for (List* i = jobs; i != NULL; i = list_next(i)) {
        job_signal(list_item(i), SIGTSTP);
    }
}

void shell_signals() {
    struct sigaction sa_sigint;
    sigset_t         mask_sigint;
    sa_sigint.sa_handler = sigint_handler;
    sa_sigint.sa_flags   = 0;
    sigfillset(&mask_sigint);
    sa_sigint.sa_mask = mask_sigint;
    sigaction(SIGINT, &sa_sigint, NULL);

    struct sigaction sa_sigtstp;
    sa_sigtstp.sa_handler = sigtstp_handler;
    sigemptyset(&sa_sigtstp.sa_mask);
    sa_sigtstp.sa_flags = 0;
    sigaction(SIGTSTP, &sa_sigtstp, NULL);

    signal(SIGCHLD, sigchld_handler);
    signal(SIGQUIT, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
}

void shell_create() {
    jobs      = list_init();
    processes = list_init();
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

void shell_add_background(Process* p) {
    // process_setpgid(p, getpid());
    int ret = setpgid(process_pid(p), getpid());
    printf("%d %d\n", ret, getpid());
}

void shell_add_job(Job* j) {
    jobs = list_push_front(jobs, j);

    processes = list_push_front(processes, job_foreground(j));

    for (int i = 0; i < job_background_size(j); i++) {
        processes = list_push_front(processes, job_background(j, i));
    }
}

void shell_wait(Process* p) {
    /*
     * Syscall waitpid may fail when an interruption occurs. Check if there
     * was any interruption and do the waitpid again
     */
    int status;
    do {
        errno  = 0;
        status = process_wait(p);
    } while (errno == EINTR);

    Job* j = shell_find_job(process_pid(p));

    if (WIFSIGNALED(status)) {
        process_complete(p, status);
        job_signal(j, WTERMSIG(status));
    }

    if (WIFSTOPPED(status)) {
        process_stop(p);
        job_signal(j, WSTOPSIG(status));
    }
}

int is_internal_op(char *command) {
    if (strcmp(command, "waitall\n") == 0) return 1;
    if (strcmp(command, "die\n") == 0) return 1;
    return 0;
}

int shell_internal_op(char *command) {
    int break_flag = 0;

    if (strcmp(command, "waitall\n") == 0) {
        shell_wait_all();
    }
    if (strcmp(command, "die\n") == 0) {
        shell_die();
        break_flag = 1;
    }

    return break_flag;
}

void shell_wait_all() {
    while (shell_child_change() != NULL);
}

void shell_die() {
    for (List* i = jobs; i != NULL; i = list_next(i)) {
        job_signal(list_item(i), SIGKILL);
    }
}

/*  Cleans out all the nodes from the processes list
    (Once all processes are already destroyed)      */
void _processes_cleaner(void* process) {}

void shell_destroy() {
    list_destroy(jobs, job_destroy_fn);
    list_destroy(processes, _processes_cleaner);
}
