#include <sys/select.h>
#include <sys/poll.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "aupe.h"


void try_select() {
    char buf[256];
    int n;
    fd_set fdset;
    FD_ZERO(&fdset);
    FD_SET(STDIN_FILENO, &fdset);
    while(1) {
        struct timeval tmval;
        tmval.tv_sec = 2;
        tmval.tv_usec = 0;
        int res = select(STDIN_FILENO + 1, &fdset, NULL, NULL, &tmval);
        if(res < 0) {
            err_sys("select failed\n");
        }
        else if(res == 0) {
            fprintf(stdout, "2 seconds passed and the stdin is not ready\n");
        }
        else {
            scanf("%s", buf);
            fprintf(stdout, "read: %s\n", buf);
            if(strcmp(buf, "quit") == 0) {
                fprintf(stdout, "quit\n");
                break;
            }
        }
    }
}

void try_poll() {
    char buf[256];
    while(1) {
        struct pollfd fdarray[1];
        fdarray[0].fd = STDIN_FILENO;
        fdarray[0].events = POLLIN;
        struct timespec tmspec;
        tmspec.tv_sec = 2;
        int res = poll(fdarray, 1, 2000);
        if(res < 0) {
            err_sys("poll failed\n");
        }
        else if(res == 0) {
            fprintf(stdout, "2 seconds passed and the stdin is not ready\n");
        }
        else {
            if(fdarray[0].revents == POLLIN) {
                scanf("%s", buf);
                fprintf(stderr, "data ready: %s\n", buf);
            }
            if(strcmp(buf, "quit") == 0) {
                fprintf(stderr, "quit\n");
                break;
            }
        }
    }
}

int main() {
    try_poll();
}