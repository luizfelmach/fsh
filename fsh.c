#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "process.h"
#include "shell.h"
#include "tokens.h"

#define COMMAND_DELIMITER "#\n"
#define ARGS_DELIMITER " "

int main(int argc, char **argv) {
    shell_create();

    char  *buffer = NULL;
    size_t bufsize;

    while (1) {
        shell_ignore_signals();
        shell_attach_fg();
        shell_prompt();
        int len = getline(&buffer, &bufsize, stdin);

        /* Captures the end of file (EOF) */
        if (len < 0) break;

        /* Separate commands by # */
        Tokens commands     = tokens_create(buffer, COMMAND_DELIMITER);
        int    commands_len = tokens_size(commands);

        /* Do nothing if no command was passed */
        if (commands_len == 0) continue;

        shell_restore_signals();

        /*  Execute 1..N-1 commands in background */
        for (int i = 1; i < commands_len; i++) {
            Tokens   bg_args = tokens_create(commands[i], ARGS_DELIMITER);
            Process *p       = process_create(bg_args, 1);
            shell_add_background(p);
        }

        /* Execute first command in foreground */
        Tokens fg_args = tokens_create(commands[0], ARGS_DELIMITER);

        if (!strcmp(fg_args[0], "waitall")) {
            printf("waitall\n");
        } else if (!strcmp(fg_args[0], "die")) {
            printf("die\n");
            /* Remove zombie processes */
            // waitpid(-1, NULL, WNOHANG);
        } else {
            Process *fg = process_create(fg_args, 0);
            shell_send_process_to_fg(fg);
        }

        shell_ignore_signals();

        /*
         * Wait for any signal
         */
        pause();

        tokens_destroy(commands);
    }

    free(buffer);
    shell_destroy();

    return 0;
}
