#include "aupe.h"
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <signal.h>

#define G_SIZE (int)1e9

const char buffer[G_SIZE];

void sig_alrm(int signo) {
    printf("receive sigalarm\n");
}

int main() {
    my_signal(SIGALRM, sig_alrm);
    alarm(1);
    FILE *file = fopen("/tmp/aupe_12", "w");
    if(file == NULL) {
        err_sys("failed to open file\n");
    }
    size_t written = fwrite(buffer, 1, G_SIZE, file);
    printf("%ld\n", written);
    return 0;
}
