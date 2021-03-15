#define SIG_LIST SIGABRT,\
SIGALRM,\
SIGBUS,\
SIGCHLD,\
SIGCONT,\
SIGFPE,\
SIGHUP,\
SIGILL,\
SIGINT,\
SIGIO,\
SIGIOT,\
SIGKILL,\
SIGPIPE,\
SIGPOLL,\
SIGPROF,\
SIGPWR,\
SIGQUIT,\
SIGSEGV,\
SIGSTKFLT,\
SIGSTOP,\
SIGSYS,\
SIGTERM,\
SIGTRAP,\
SIGTSTP,\
SIGTTIN,\
SIGTTOU,\
SIGURG,\
SIGUSR1,\
SIGUSR2,\
SIGVTALRM,\
SIGWINCH,\
SIGXCPU,\
SIGXFSZ,\

#include "aupe.h"
#include <signal.h>
#include <errno.h>
#include <stdio.h>

const int sig_list[] = {
    SIG_LIST
};

void pr_mask(const char *str) {
    sigset_t sigset;
    error_t save_err;
    char buffer[256];
    
    save_err = errno;

    if(sigprocmask(0, NULL, &sigset) < 0) {
        err_sys("failed to run sigprocmask\n");
    }
    else {
        for(int i = 0; i < sizeof(sig_list) / sizeof(sig_list[0]); i++) {
            if(sigismember(&sigset, sig_list[i])) {
                sig2str(sig_list[i], buffer);
                printf("%s ", buffer);
            }
        }
    }
    errno = save_err;
}

int main() {
    pr_mask("ch10: p9");
    return 0;
}