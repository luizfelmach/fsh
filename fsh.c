#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "job.h"
#include "shell.h"

static char  *buffer  = NULL; /* Current prompt */
static size_t bufsize = 0;    /* Size of buffer */
static int    nread   = 0;    /* Getline return */

void shell_loop();

int main(int argc, char **argv) {
    shell_create();
    shell_loop();
    shell_destroy();

    return EXIT_SUCCESS;
}

void shell_loop() {
    for (;;) {
        shell_signals();
        shell_attach_fg();
        shell_prompt();

        /*
         * Syscall getline may fail when an interruption occurs. Check if there
         * was any interruption and do the getline again
         */
        do {
            errno = 0;
            nread = getline(&buffer, &bufsize, stdin);
            if (errno == EINTR) clearerr(stdin);
        } while (errno == EINTR);

        char *command = strdup(buffer);

        /* Captures the end of file (EOF) */
        if (nread < 0) break;

        /* Checking shell commands */

        if (strcmp(command, "waitall\n") == 0) {
            shell_wait_all();
            continue;
        }
        if (strcmp(command, "die\n") == 0) {
            shell_die();
            break;
        }

        Job     *job = job_init(command);
        Process *fg  = job_spawn(job);

        if (fg == NULL) {
            /* No processes were executed */
            job_destroy(job);
            continue;
        }

        shell_add_job(job);

        shell_wait(fg);

        free(command);
    }

    free(buffer);
}