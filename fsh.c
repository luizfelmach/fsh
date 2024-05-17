#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
    char *file, *args[10];
    bool foreground;
} command;

command command_init();

pid_t process_spawn(command *);

int main() {
    command commands[2];

    commands[0] = (command){
        .file = "ls",
        .args = {"ls", "-la", NULL},
        .foreground = true,
    };

    commands[1] = (command){
        .file = "ps",
        .args = {"ps", NULL},
        .foreground = true,
    };

    process_spawn(commands);
    process_spawn(commands + 1);

    return 0;
}

void loop() {
    char *buffer = NULL;
    size_t bufsize = 32;

    while (1) {
        printf("fsh>");
        getline(&buffer, &bufsize, stdin);
        buffer[2] = '\0';
        // process_spawn("pstree");
    }
}

pid_t process_spawn(command *command) {
    pid_t pid = fork();
    if (pid == -1) perror("error");
    if (pid == 0) {
        if (execvp(command->file, command->args) == -1) perror("error");
    }
    return pid;
}
