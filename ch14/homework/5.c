/* use select or poll to implement sleep_us */
#include "aupe.h"
#include <stdio.h>
#include <sys/select.h>
#include <sys/poll.h>
#include <sys/time.h>

void sleep_us_select(int us) {
    struct timeval tmval;
    tmval.tv_sec = 0;
    tmval.tv_usec = us;
    if(select(0, NULL, NULL, NULL, &tmval) < 0) {
        err_sys("select failed\n");
    }
}

void sleep_us_poll(int us) {
    int time_out = us / 1000;
    if(poll(NULL, 0, time_out) < 0) {
        err_sys("poll failed\n");
    }
}

int main() {
    printf("before sleep 1 sec\n");
    sleep_us_poll(1000000);
    printf("sleep 1 sec again\n");
    sleep_us_select(1000000);
    printf("awaken!\n");
    return 0;
}