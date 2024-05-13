#include <stdio.h>
#include <unistd.h>

void process_spawn(char *);

int main() {
    char *buffer = NULL;
    size_t bufsize = 32;

    while (1) {
        printf("fsh>");
        getline(&buffer, &bufsize, stdin);
        buffer[2] = '\0';
        process_spawn("pstree");
    }

    return 0;
}

void process_spawn(char *command) {
    pid_t pid = fork();
    if (pid == -1) perror("error");
    if (pid == 0) {
        if (execlp(command, command, NULL) == -1) perror("error");
    }
}
