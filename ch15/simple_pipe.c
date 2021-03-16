#include "aupe.h"
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd[2];
    pid_t pid;
    if(pipe(fd) < 0) {
        err_sys("failed to create pipe\n");
    }
    if((pid = fork()) < 0) {
        err_sys("failed to call fork\n");
    }
    else if(pid == 0) {  // children write
        close(fd[0]);
        write(fd[1], "hello world\n", sizeof("hello world\n"));
        close(fd[1]);
        return 0;
    }
    else {
        char buffer[256];
        close(fd[1]);
        int n = read(fd[0], buffer, sizeof(buffer));
        if(n < 0) {
            err_sys("failed to read pipe\n");
        }
        buffer[n] = '\0';
        printf("%s", buffer);
        close(fd[0]);
    }
    return 0;
}