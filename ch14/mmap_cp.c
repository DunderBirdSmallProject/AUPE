// copy through mmap
#include "aupe.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if(argc != 3) {
        err_sys("usage: %s <fromfile> <tofile>\n", argv[0]);
    }
    int fdin, fdout;
    if((fdin = open(argv[1], O_RDONLY)) < 0) {
        err_sys("can't open %s\n", argv[1]);
    }
    if((fdout = open(argv[2], O_RDWR | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR)) < 0) {
        err_sys("can't write/create %s\n", argv[2]);
    }

    struct stat sbuf;
    if(fstat(fdin, &sbuf) < 0) {
        err_sys("fstat error\n");
    }
    if(ftruncate(fdout, sbuf.st_size) < 0) {
        err_sys("failed to truncate %s\n", argv[2]);
    }

    void *src, *dst;
    if((src = mmap(NULL, sbuf.st_size, PROT_READ, MAP_SHARED, fdin, 0)) == MAP_FAILED) {
        err_sys("failed to map %s\n", argv[1]);
    }
    if((dst = mmap(0, sbuf.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fdout, 0)) == MAP_FAILED) {
        err_sys("failed to map %s\n", argv[2]);
    }

    memcpy(dst, src, sbuf.st_size);
    munmap(src, sbuf.st_size);
    munmap(dst, sbuf.st_size);
}