#include "aupe.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd;
    int cnt = 0;
    pid_t pid;
    char buffer[256];
    if((fd = open("ch10_6.txt", O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR)) < 0) {
        err_sys("failed to open ch10_6\n");
    }
    write(fd, "0\n", 2);
    TELL_WAIT();
    if((pid = fork()) < 0) {
        err_sys("failed to fork");
    }
    else if(pid > 0) {
        int next = 2;
        while(cnt < 3) {
            WAIT_CHILDREN();
            sprintf(buffer, "%d", next);
            strcat(buffer, " father\n");
            if(write(fd, buffer, strlen(buffer)) < 0) {
                err_sys("father write error\n");
            }
            TELL_CHILD(pid);
            next += 2;
            cnt++;
        }
    }
    else {
        int next = 1;
        while(cnt < 3) {
            sprintf(buffer, "%d", next);
            strcat(buffer, " children\n");
            if(write(fd, buffer, strlen(buffer)) < 0) {
                err_sys("child write error\n");
            }
            TELL_PARENT(getppid());
            WAIT_PARENT();
            next += 2;
            cnt++;
        }
    }
    
    return 0;
}