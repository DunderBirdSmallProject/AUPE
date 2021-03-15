// https://github.com/adalton/apue3/tree/master/Chapter14
#include "aupe.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>


void lock(int fd, short type) {
    struct flock f_lock;
    f_lock.l_len = 1;
    f_lock.l_whence = SEEK_SET;
    f_lock.l_start = 0;
    f_lock.l_type = type;
    
    if(fcntl(fd, F_SETLKW, &f_lock) < 0) {
        err_sys("failed to lock\n");
    }
}

void unlock(int fd) {
    struct flock f_lock;
    f_lock.l_len = 1;
    f_lock.l_whence = SEEK_SET;
    f_lock.l_start = 0;
    f_lock.l_type = F_SETLK;

    if(fcntl(fd, F_UNLCK, &f_lock) < 0) {
        err_sys("failed to unlock");
    }
}

void reader(int fd) {
    lock(fd, F_RDLCK);
}

void writer(int fd) {
    lock(fd, F_WRLCK);
}

int main(int argc, char *argv[]) {
    if(argc <= 1) {
        perror("please enter a filename");
    }

    int fd = open(argv[1], O_RDWR);
    pid_t pid = 0;

    if(fd < 0) {
        err_sys("failed to fork\n");
    }
    if((pid = fork()) == 0) {
        printf("children1 try to get read lock\n");
        reader(fd);
        printf("children1 get reader lock\n");
        sleep(10);
        unlock(fd);
        printf("children1 unlocked\n");
        close(fd);
        return 0;
    }

    sleep(2);  // make sure children runs

    if((pid = fork()) == 0) {
        sleep(3);  // father run first to get write lock
        printf("children2 try to get read lock\n");
        reader(fd);
        printf("children2 get reader lock\n");
        sleep(8);
        unlock(fd);
        printf("children2 unlocked\n");
        close(fd);
        return 0;
    }

    printf("father try to get write lock\n");
    writer(fd);
    printf("father get lock\n");
    sleep(3);
    unlock(fd);
    printf("father unlocked\n");
    close(fd);
    
    
    wait(NULL);
    wait(NULL);
    return 0;
}
