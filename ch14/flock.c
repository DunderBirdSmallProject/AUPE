#include "aupe.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

static void lockabyte(const char *name, int fd, off_t offset) {
    if(writew_flock(fd, offset, SEEK_SET, 1) < 0) {
        err_sys("%s: writew_lock error\n", name);
    }
    printf("%s got the lock, byte %lld\n", name, (long long)offset);
}

void deadlock() {
    pid_t pid;
    int fd;

    if((fd = creat("deadlock.tmp", O_RDWR)) < 0) {
        err_sys("creat error");
    }
    if(write(fd, "ab", 2) != 2) {
        err_sys("write error");
    }

    TELL_WAIT();
    
    if((pid = fork()) < 0) {
        err_sys("failed to create child process");
    }
    else if(pid == 0) {  /* children */
        lockabyte("child", fd, 0);
        TELL_PARENT(getppid());
        WAIT_PARENT();
        lockabyte("child", fd, 1);
    }
    else {
        lockabyte("parent", fd, 1);
        TELL_CHILD(pid);
        WAIT_PARENT();
        lockabyte("parent", fd, 0);
    }    
}

int main() {
    deadlock();
}
