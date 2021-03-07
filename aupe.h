#ifndef AUPE_H
#define AUPE_H

#ifndef __USE_POSIX
#define __USE_POSIX
#endif

#ifndef _POSIX_SOURCE
#define _POSIX_SOURCE
#endif

#include <fcntl.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/types.h>

/**
 * Error
 */
void err_sys(const char *msg, ...);

/**
 * IPC
 */
void TELL_WAIT();
void TELL_PARENT(pid_t pid);
void WAIT_PARENT();
void TELL_CHILD(pid_t pid);


/*
 * IO
 */
ssize_t readn(int fd, void *ptr, size_t n);
ssize_t writen(int fd, void *ptr, size_t n);


void set_fl(int fd, int flags);  /* fcntl internally */
void clr_fl(int fd, int flags);

int flock_reg(int fd, int cmd, int type, off_t offset, int whence, off_t len);
#define read_flock(fd, offset, whence, len) \
    flock_reg((fd), F_SETLK, F_RDLCK, (offset), (whence), (len))
#define readw_flock(fd, offset, whence, len) \
    flock_reg((fd), F_SETLKW, F_RDLCK, (offset), (whence), (len))
#define write_flock(fd, offset, whence, len) \
    flock_reg((fd), F_SETLK, F_WRLCK, (offset), (whence), (len))
#define writew_flock(fd, offset, whence, len) \
    flock_reg((fd), F_SETLKW, F_WRLCK, (offset), (whence), (len))
#define un_flock(fd, offset, whence, len) \
    flock_reg((fd), F_SETLK, F_UNLCK, (offset), (whence), (len))
pid_t test_flock(int fd, int type, off_t offset, int whence, off_t len);
#define is_read_lockable(fd, offset, whence, len) \
    (test_flock((fd), F_RDLCK, (offset), (whence), (len)) == 0)
#define is_write_lockable(fd, offset, whence, len) \
    (test_flock((fd), F_WRLCK, (offset), (whence), (len)) == 0)

#endif