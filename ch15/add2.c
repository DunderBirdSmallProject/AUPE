#include "aupe.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/fcntl.h>

#define MAXLEN 256

int main(int argc, char *argv[]) {
    char buffer[MAXLEN];
    int n, int1, int2;

    while((n = read(STDIN_FILENO, buffer, MAXLEN-1)) > 0) {
        buffer[n] = 0;
        if(sscanf(buffer, "%d%d", &int1, &int2) == 2) {
            sprintf(buffer, "%d\n", int1 + int2);
            n = strlen(buffer);
            if(write(STDOUT_FILENO, buffer, n) != n) {
                err_sys("write error\n");
            }
        }
        else {
            const int sz = sizeof("invalid args\n");
            if((write(STDOUT_FILENO, "invalid args\n", sz)) != sz) {
                err_sys("write error\n");
            }
        }
    }
    return 0;
}