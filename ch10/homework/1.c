#include "aupe.h"
#include <unistd.h>
#include <signal.h>
#include <stdio.h>


static void sig_usr(int signo);

int main() {
    if(signal(SIGUSR1, sig_usr) < 0) {
        err_sys("USR1 error\n");
    }
    if(signal(SIGUSR2, sig_usr) < 0) {
        err_sys("USR2 error\n");
    }
    pause();
}

void sig_usr(int signo) {
    if(signo == SIGUSR1) {
        printf("receive USR1\n");
    }
    else if(signo == SIGUSR2) {
        printf("receive USR2\n");
    }
    else {
        err_sys("shouldn't reach here\n");
    }
}
