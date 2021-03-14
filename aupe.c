#include "aupe.h"
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>


void err_sys(const char *msg, ...) {
    va_list ap;
    va_start(ap, msg);
    vfprintf(stderr, msg, ap);
    va_end(ap);
    exit(-1);
}


/**
 * Signal
 */
SigHandler* my_signal(int signo, SigHandler* handler) {
    struct sigaction act, oact;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if(signo == SIGALRM) {
        #ifdef SA_INTERRUPT
        act.sa_flags |= SA_INTERRUPT;
        #endif
    }
    else {
        act.sa_flags |= SA_RESTART;
    }

    if(sigaction(signo, &act, &oact) < 0) {
        return SIG_ERR;
    }
    return oact.sa_handler;
}

/* https://github.com/adalton/apue3/tree/master/Chapter10 */

#define SIG_MACRO(macro) \
    macro(SIGHUP) \
	macro(SIGINT) \
	macro(SIGQUIT) \
	macro(SIGILL) \
	macro(SIGILL) \
	macro(SIGTRAP) \
	macro(SIGABRT) \
	macro(SIGBUS) \
	macro(SIGFPE) \
	macro(SIGKILL) \
	macro(SIGUSR1) \
	macro(SIGSEGV) \
	macro(SIGUSR2) \
	macro(SIGPIPE) \
	macro(SIGALRM) \
	macro(SIGTERM) \
	macro(SIGSTKFLT) \
	macro(SIGCHLD) \
	macro(SIGCONT) \
	macro(SIGSTOP) \
	macro(SIGTSTP) \
	macro(SIGTTIN) \
	macro(SIGTTOU) \
	macro(SIGURG) \
	macro(SIGXCPU) \
	macro(SIGXFSZ) \
	macro(SIGVTALRM) \
	macro(SIGPROF) \
	macro(SIGWINCH) \
	macro(SIGIO) \
	macro(SIGPWR) \
	macro(SIGSYS)

#define SIG_ARRAY(sig) [sig] = #sig,

static const char* sig_name[] = {
    SIG_MACRO(SIG_ARRAY)
};

int sig2str(const int signo, char* const str) {
    if(signo < 0 || signo > sizeof(sig_name) / sizeof(sig_name[0])) {
        return -1;
    }
    if(sig_name[signo] == NULL) {
        return -1;
    }
    strcpy(str, sig_name[signo]);
    return 0;
}

/**
 * IPC related
 */
static volatile sig_atomic_t sigflag;
static sigset_t newmask, oldmask, zeromask;

static void sig_usr(int signo) {
    sigflag = 1;
}

void TELL_WAIT() {
    if(signal(SIGUSR1, sig_usr) == SIG_ERR) {
        err_sys("signal(SIGUSR1) error");
    }
    if(signal(SIGUSR2, sig_usr) == SIG_ERR) {
        err_sys("signal(SIGUSR2) error");
    }
    sigemptyset(&zeromask);
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGUSR1);
    sigaddset(&newmask, SIGUSR2);

    if(sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) {
        err_sys("SIG_BLOCK error");
    }
}

void TELL_PARENT(pid_t pid) {
    kill(pid, SIGUSR2);
}

void WAIT_PARENT() {
    while (sigflag == 0) {
        sigsuspend(&zeromask);
    }
    sigflag = 0;
    if(sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0) {
        err_sys("SIGSET_MASK error");
    }
}

void TELL_CHILD(pid_t pid) {
    kill(pid, SIGUSR1);
}


/*
 * IO
 */
ssize_t readn(int fd, void *ptr, size_t n) {
    size_t nleft = n;
    ssize_t cur_read = 0;
    
    while(nleft > 0) {
        if((cur_read = read(fd, ptr, nleft)) < 0) {
            if(nleft == n) {
                return -1;
            }
            else {
                break;
            }
        }
        else if(cur_read == 0) {
            break;
        }
        nleft -= cur_read;
        ptr += cur_read;
    }
    return (n - nleft);
}

ssize_t writen(int fd, void *ptr, size_t n) {
    size_t nleft = n;
    ssize_t cur_write = 0;
    
    while(nleft > 0) {
        if((cur_write = write(fd, ptr, nleft)) < 0) {
            if(nleft == n) {
                return -1;
            }
            else {
                break;
            }
        }
        else if(cur_write == 0) {
            break;
        }
        nleft -= cur_write;
        ptr += cur_write;
    }
    return (n - nleft);
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

int filelock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len) {
    struct flock lock;
    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    return fcntl(fd, cmd, &lock);
}

pid_t test_filelock(int fd, int type, off_t offset, int whence, off_t len) {
    struct flock lock;

    lock.l_type = type;
    lock.l_start = offset;
    lock.l_whence = whence;
    lock.l_len = len;

    if(fcntl(fd, F_GETLK, &lock) < 0) {
        err_sys("fcntl error\n");
    }

    if(lock.l_type == F_UNLCK) {
        return 0;
    }
    return lock.l_pid;
}
