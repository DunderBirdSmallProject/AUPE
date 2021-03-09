#include "aupe.h"
#include <signal.h>
#include <errno.h>
#include <stdio.h>

void pr_mask() {
    sigset_t sigset;
    int errno_save;
    errno_save = errno;
    if(sigprocmask(0, NULL, &sigset) < 0) {
        err_sys("sigprocmask failed");
    }
    else {
        if(sigismember(&sigset, SIGINT)) {
            printf("SIGINT\n");
        }
        if(sigismember(&sigset, SIGUSR1)) {
            printf("SIGUSR1\n");
        }
        if(sigismember(&sigset, SIGQUIT)) {
            printf("SIGQUIT\n");
        }
    }
}

int main() {
    pr_mask();
}