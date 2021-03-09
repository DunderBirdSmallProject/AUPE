/* a simple use of signal */
#include "aupe.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

static void sig_usr(int signo);

int main() {
    if(signal(SIGUSR1, sig_usr) == SIG_ERR) {
        err_sys("failed to set handler for SIGUSR1\n");
    }
    if(signal(SIGUSR2, sig_usr) == SIG_ERR) {
        err_sys("failed to set handler for SIGUSR2\n");
    }
    while(1) {
        pause();
    }
    return 0;
}

static void sig_usr(int signo) {
    if(signo == SIGUSR1) {
        printf("receive signal usr1\n");
    }
    else if(signo == SIGUSR2) {
        printf("receive signal usr2\n");
    }
    else {
        printf("receive other signal\n");
    }
}