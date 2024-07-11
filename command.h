#ifndef COMMAND_H
#define COMMAND_H

typedef struct command Command;

Command *command_create(char *str);
void command_destroy(Command *cmd);

#endif  // !COMMAND_H
