#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "shell.h"
#include "tokens.h"

#define COMMAND_DELIMITER "#\n"
#define ARGS_DELIMITER " "

int main(int argc, char **argv) {
    shell_create();

    char  *buffer = NULL;
    size_t bufsize;

    while (1) {
        shell_prompt();
        int len = getline(&buffer, &bufsize, stdin);

        /* Captures the end of file (EOF) */
        if (len < 0) break;

        /* Separate commands by # */
        Tokens commands     = tokens_create(buffer, COMMAND_DELIMITER);
        int    commands_len = tokens_size(commands);

        /* Do nothing if no command was passed */
        if (commands_len == 0) continue;

        /*  Execute 1..N-1 commands in background */
        for (int i = 1; i < commands_len; i++) {
            Tokens bg_args = tokens_create(commands[i], ARGS_DELIMITER);
            shell_execute_bg(bg_args);
        }

        /* Execute first command in foreground and wait for process */
        Tokens fg_args = tokens_create(commands[0], ARGS_DELIMITER);
        shell_execute_fg(fg_args);
    }

    /* Clean up memory */
    free(buffer);
    shell_destroy();

    return 0;
}
