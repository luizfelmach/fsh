#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tokens tokens_create(char *s, char *delimiter) {
    int s_len       = strlen(s);
    int n_delimiter = 0;
    int i           = 0;

    for (i = 0; i < s_len; i++)
        if (s[i] == *delimiter) n_delimiter += 1;

    char **result = calloc((n_delimiter + 2), sizeof(char **));

    if (n_delimiter == 0) return result;

    /* Set the last position NULL */
    result[n_delimiter - 1] = NULL;

    char *token = strtok(s, delimiter);

    for (i = 0; i < n_delimiter + 1; i++) {
        result[i] = strdup(token);
        token     = strtok(NULL, delimiter);
    }

    return result;
}

int tokens_size(Tokens t) {
    int size = 0;
    int i    = 0;
    while (t[i++] != NULL) size++;
    return size;
}

void tokens_destroy(Tokens t) {
    int tokens_len = tokens_size(t);
    for (int i = 0; i < tokens_len; i++) free(t[i]);
    free(t);
}
