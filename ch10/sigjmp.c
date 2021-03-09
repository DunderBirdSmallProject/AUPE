#include "aupe.h"
#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <time.h>

void pr_mask(const char *str) {
    sigset_t sigset;
    int errno_save;
    errno_save = errno;
    printf("%s:\n", str);
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
        if(sigismember(&sigset, SIGALRM)) {
            printf("SIGALARM\n");
        }
    }
    errno = errno_save;
}

static void sig_alarm(int signo);
static void sig_usr1(int signo);
static sigjmp_buf jmpbuf;
static volatile sig_atomic_t canjump;

int main() {
    if(signal(SIGUSR1, sig_usr1) == SIG_ERR) {
        err_sys("failed to register SIGUSR1\n");
    }
    if(signal(SIGALRM, sig_alarm) == SIG_ERR) {
        err_sys("failed to register alarm\n");
    }
    pr_mask("starting main");
    if(sigsetjmp(jmpbuf, 1)) {
        pr_mask("ending main");
        exit(0);
    }
    canjump = 1;
    for(;;)
        pause();
    return 0;
}

static void sig_alarm(int signo) {
    pr_mask("alarm process");
}

static void sig_usr1(int signo) {
    time_t start_time;
    
    if(canjump == 0) {
        return;
    }
    pr_mask("starting sig_usr1");

    alarm(3);
    start_time = time(NULL);

    while(time(NULL) < start_time + 5);

    pr_mask("finishing sig_usr1");
    canjump = 0;
    siglongjmp(jmpbuf, 1);
}
