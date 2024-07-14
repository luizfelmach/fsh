#include <stdio.h>
#include <stdlib.h>

#include "process.h"
#include "shell.h"
#include "tokens.h"
#include "utils.h"

int main(int argc, char **argv) {
    shell_init();

    char  *buffer = NULL;
    size_t bufsize;
    int    len;

    while (1) {
        /* Show prompt */
        printf("fsh > ");

        len = getline(&buffer, &bufsize, stdin);

        /* Captures the end of line (EOF) */
        if (len < 0) break;

        /* Remove \n from the last position of the string */
        buffer[len - 1] = '\0';

        Tokens args     = tokens_create(buffer, " ");
        int    args_len = tokens_size(args);

        printf("LEN: %d\n", args_len);

        for (int i = 0; i < args_len; i++) {
            printf("%d - '%s'\n", i, args[i]);
        }

        Process *p = process_fg(args);
        process_spawn(p);
        process_wait(p);
        tokens_destroy(args);
    }

    free(buffer);

    return 0;
}
