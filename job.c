#include "job.h"

#define COMMAND_DELIMITER "#\n"
#define ARGS_DELIMITER " "

struct job {
    char    *command;         /* Command e.g.: ls -la # ping localhost */
    int      bg_size;         /* Amount of background processes */
    pid_t    pgbgid;          /* PGID of background processes */
    Process *foreground;      /* Pointer to foreground process*/
    Process *background[256]; /* Array of processes in background */
};

Job *job_init(char *command) {
    Job *j     = calloc(1, sizeof(Job));
    j->command = strdup(command);
    j->bg_size = 0;
    j->pgbgid  = 0;
    return j;
}

Process *job_spawn(Job *j) {
    /* Separate process by # */
    Tokens process     = tokens_create(j->command, COMMAND_DELIMITER);
    int    process_len = tokens_size(process);

    if (process_len == 0) return NULL;

    Tokens fg_args = tokens_create(process[0], ARGS_DELIMITER);
    j->foreground  = process_create(fg_args, 0, 0);

    /* Set PGID to foreground */
    tcsetpgrp(STDOUT_FILENO, process_pid(j->foreground));

    /*  Execute 1..N-1 process in background */
    for (int i = 1; i < process_len; i++) {
        Tokens bg_args = tokens_create(process[i], ARGS_DELIMITER);

        j->background[j->bg_size] = process_create(bg_args, j->pgbgid, 1);
        j->bg_size += 1;

        /* Set first background process as PGID */
        if (j->pgbgid == 0) {
            j->pgbgid = process_pid(j->background[j->bg_size - 1]);
        }
    }

    tokens_destroy(process);

    return j->foreground;
}

void job_signal(Job *j, int sig) {
    kill(process_pid(j->foreground), sig);
    if (j->bg_size >= 1) kill(-process_pid(j->background[0]), sig);
}

Process *job_contains(Job *j, pid_t pid) {
    if (process_pid(j->foreground) == pid) return j->foreground;

    for (int i = 0; i < j->bg_size; i++) {
        if (process_pid(j->background[i]) == pid) return j->background[i];
    }

    return NULL;
}

int job_active(Job *j) {
    if (!process_completed(j->foreground)) return 1;

    for (int i = 0; i < j->bg_size; i++) {
        if (!process_completed(j->background[i])) return 1;
    }

    return 0;
}

Process *job_foreground(Job *j) {
    return j->foreground;
}

Process *job_background(Job *j, int pos) {
    return j->background[pos];
}

int job_background_size(Job *j) {
    return j->bg_size;
}

void job_destroy(Job *j) {
    for (int i = 0; i < j->bg_size; i++) {
        process_destroy(j->background[i]);
    }
    process_destroy(j->foreground);
    free(j->command);
    free(j);
}

void job_destroy_fn(void *j) {
    job_destroy((Job*) j);
}
