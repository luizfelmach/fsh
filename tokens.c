#include "tokens.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

#define TOKENS_MAX 1024

Tokens tokens_create(char *s, char *delimiter) {
    char **result = calloc(TOKENS_MAX + 1, sizeof(char **));
    char  *token  = strtok(s, delimiter);
    int    i      = 0;

    while (token != NULL) {
        result[i++] = strdup(token);
        token       = strtok(NULL, delimiter);
    }

    result[i] = NULL;

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
