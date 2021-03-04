#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "aupe.h"

void err_sys(const char *msg) {
    fprintf(stderr, msg);
    exit(-1);
}

void set_fl(int fd, int flags) {
    int val;
    if((val = fcntl(fd, F_GETFL)) < 0) {
        err_sys("fcntl F_GETFL error\n");
    }

    val |= flags;

    if(fcntl(fd, F_SETFD, val) < 0) {
        err_sys("fcntl F_SETFL error\n");
    }
}

void clr_fl(int fd, int flags) {
    int val;
    if((val = fcntl(fd, F_GETFL)) < 0) {
        err_sys("fcntl F_GETFL error\n");
    }

    val ^= flags;

    if(fcntl(fd, F_SETFD, val) < 0) {
        err_sys("fcntl F_SETFL error\n");
    }
}
