#include "aupe.h"
#include <errno.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int init_server(int type, const struct sockaddr *addr, socklen_t alen, int qlen) {
    int fd = 0, err = 0;

    if((fd = socket(addr->sa_family, type, 0)) < 0) {
        return -1;
    }
    if(bind(fd, addr, alen) < 0) {
        goto errout;
    }
    if(type == SOCK_STREAM || type == SOCK_SEQPACKET) {
        if(listen(fd, qlen) < 0)
            goto errout;
    }
    return fd;

errout:
    err = errno;
    close(fd);
    errno = err;
    return -1;
}
