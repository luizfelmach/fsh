#include <stdio.h>
#include <stdlib.h>

#include "shell.h"

int main() {
    shell_init();

    char *buffer = NULL;
    size_t bufsize;
    int len;

    while (1) {
        /* Show prompt */
        printf("fsh > ");

        len = getline(&buffer, &bufsize, stdin);

        /* Captures the end of line (EOF) */
        if (len < 0) break;

        /* Remove \n from the last position of the string */
        buffer[len - 1] = '\0';
    }

    return 0;
}
