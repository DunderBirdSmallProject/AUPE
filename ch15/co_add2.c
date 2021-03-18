#include "aupe.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

void sig_pipe(int signo);

int main() {
    if(my_signal(SIGPIPE, sig_pipe) == SIG_ERR) {
        err_sys("failed to register pipe function\n");
    }

    pid_t pid;
    int fd1[2], fd2[2];
    if(pipe(fd1) < 0 || pipe(fd2) < 0) {
        err_sys("pipe error\n");
    }

    if((pid = fork()) < 0) {
        err_sys("fork error\n");
    }
    else if(pid == 0) {
        close(fd1[1]);
        close(fd2[0]);

        if(fd1[0] != STDIN_FILENO) {
            if(dup2(fd1[0], STDIN_FILENO) < 0) {
                err_sys("failed to re-direct stdin\n");
            }
            close(fd1[0]);
        }
        if(fd2[1] != STDOUT_FILENO) {
            if(dup2(fd2[1], STDOUT_FILENO) < 0) {
                err_sys("failed to re-direct stdout\n");
            }
            close(fd2[1]);
        }
        if(execl("./add2", "add2", (char *)0) < 0) {
            err_sys("execl error");
        }
        exit(-1);
    }
    else {
        close(fd1[0]);
        close(fd2[1]);
        char line[256];

        while(fgets(line, 256, stdin) != NULL) {
            int n = strlen(line);
            if(write(fd1[1], line, n) != n) {
                err_sys("write error to pipe\n");
            }
            if((n = read(fd2[0], line, 255)) < 0) {
                err_sys("read pipe failed\n");
            }
            if(n == 0) {
                err_sys("children closed pipe\n");
            }
            line[n] = 0;
            if(fputs(line, stdout) == EOF) {
                err_sys("fputs error\n");
            }
        }

        if(ferror(stdin)) {
            err_sys("fgets error on stdin\n");
        }
        exit(0);
    }

    return 0;
}

void sig_pipe(int signo) {
    printf("receive SIGPIPE\n");
    exit(1);
}