#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <stdio.h>

int main() {
    int cnt = 0;
    do {
        if(cnt == 4) {
            time_t now;
            struct tm *tmnow;
            time(&now);
            tmnow = localtime(&now);
            printf("%d\n", tmnow->tm_sec);
        }
        sleep(60);
        cnt += 1;
    } while(1);
}