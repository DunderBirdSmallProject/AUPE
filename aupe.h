#ifndef AUPE_H
#define AUPE_H

#ifndef __USE_POSIX
#define __USE_POSIX
#endif

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif

#ifndef __USE_UNIX98
#define __USE_UNIX98
#endif

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200112L
#endif

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#define SIG_NAME_MAXLEN 10

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>

/* Error */
void err_sys(const char *msg, ...);


/* Signal */
typedef void SigHandler(int);
SigHandler* my_signal(int signo, SigHandler* handler);
/**
 * @return -1 for unsucessful, 0 for successful.
 */
int sig2str(const int signo, char* const str);


/* IPC */
void TELL_WAIT();
void TELL_PARENT(pid_t pid);
void WAIT_PARENT();
void TELL_CHILD(pid_t pid);
void WAIT_CHILDREN();


/* IO */
ssize_t readn(int fd, void *ptr, size_t n);
ssize_t writen(int fd, void *ptr, size_t n);


void set_fl(int fd, int flags);
void clr_fl(int fd, int flags);

int filelock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);
#define read_filelock(fd, offset, whence, len) \
    filelock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_filelock(fd, offset, whence, len) \
    filelock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_filelock(fd, offset, whence, len) \
    filelock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_filelock(fd, offset, whence, len) \
    filelock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_filelock(fd, offset, whence, len) \
    filelock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))
pid_t test_filelock(int fd, int type, off_t offset, int whence, off_t len);
#define is_read_lockable(fd, offset, whence, len) \
    (test_filelock((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
    (test_filelock((fd), F_WRLCK, (offset), (whence), (len)) == 0)

#endif