#include "aupe.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>

struct ListNode {
    struct ListNode *next;
    time_t timestamp;
    SigHandler *handler;
};


static struct ListNode* event_list_head;
SigHandler *old_handler;


SigHandler* timer_signal(SigHandler *handler) {
    struct sigaction act, oact;
    const int signo = SIGALRM;
    act.sa_handler = handler;
    #ifdef SA_INTERRUPT
    act.sa_flags |= SA_INTERRUPT;
    #endif
    if(sigaction(signo, &act, &oact) < 0) {
        err_sys("failed to register signal\n");
    }
    return oact.sa_handler;
}

void _time_manager_handler(int signo) {
    if(signo == SIGALRM) {
        struct timespec cur_time;
        clock_gettime(CLOCK_REALTIME, &cur_time);
        struct ListNode *tmp;
        while(event_list_head != NULL) {
            if(event_list_head->timestamp <= cur_time.tv_sec) {
                event_list_head->handler(signo);
                tmp = event_list_head;
                event_list_head = event_list_head->next;
                free(tmp);
            }
            else {
                break;
            }
        }
        if(event_list_head == NULL) {
            timer_signal(old_handler);  // restore old handler
            old_handler = NULL;
        }
        else {
            alarm(event_list_head->timestamp - cur_time.tv_sec);
        }
    }
}

void init_time_manager() {
    event_list_head = NULL;
}

void register_alarm_event(int seconds, SigHandler *handler) {
    struct timespec cur_time;
    clock_gettime(CLOCK_REALTIME, &cur_time);
    const int abs_time = cur_time.tv_sec + seconds;
    struct ListNode *new_node = (struct ListNode*)malloc(sizeof(struct ListNode));
    new_node->timestamp = abs_time;
    new_node->handler = handler;
    new_node->next = NULL;
    if(event_list_head == NULL) {
        event_list_head = new_node;
        old_handler = timer_signal(_time_manager_handler);
    }
    else {
        if(abs_time < event_list_head->timestamp) {
            new_node->next = event_list_head;
            event_list_head = new_node;
        }
        else {
            struct ListNode *cur = event_list_head;
            while(cur->next != NULL) {
                if(cur->next->timestamp > abs_time) {
                    break;
                }
            }
            new_node->next = cur->next;
            cur->next = new_node;
        }
    }
    alarm(event_list_head->timestamp - cur_time.tv_sec);
}

static void print_1s(int signo) {
    printf("receive SIGALARM after 1 second\n");
}

static void print_2s(int signo) {
    printf("receive SIGALARM after 2 seconds\n");
}

static void print_3s(int signo) {
    printf("receive SIGALARM after 3 seconds\n");
}

int main() {
    init_time_manager();
    register_alarm_event(3, print_3s);
    register_alarm_event(1, print_1s);
    register_alarm_event(2, print_2s);
    for(int i = 0; i < 3; ++i) {
        pause();
    }
    register_alarm_event(1, print_1s);
    pause();
    register_alarm_event(2, print_2s);
    pause();
    register_alarm_event(3, print_3s);
    pause();
    printf("receive all SIGALARM signals\n");
    return 0;
}