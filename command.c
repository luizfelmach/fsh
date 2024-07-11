#include "command.h"

#include <stdlib.h>

struct command {
    char **args;
};

Command *command_create(char *str) {
    Command *cmd = malloc(sizeof(Command));
    return cmd;
}

void command_destroy(Command *cmd) {
    free(cmd);
}
