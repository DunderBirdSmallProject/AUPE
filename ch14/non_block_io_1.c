#include <fcntl.h>
#include <error.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include "aupe.h"

char buf[500000];

int main() {
    int ntowrite, nwrite;
    char *ptr;
    ntowrite = read(STDIN_FILENO, buf, sizeof(buf));
    fprintf("read %d bytes\n", ntowrite);

    set_fl(STDOUT_FILENO, O_NONBLOCK);

    ptr = buf;
    while(ntowrite > 0) {
        errno = 0;
        nwrite = write(STDOUT_FILENO, buf, ntowrite);
        fprintf(stderr, "nwrite = %d, errno = %d\n", nwrite, errno);
        if (nwrite > 0) {
            ntowrite -= nwrite;
            ptr += nwrite;
        }
    }
    clr_fl(STDOUT_FILENO, O_NONBLOCK);
    return 0;
}