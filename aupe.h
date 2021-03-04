#ifndef AUPE_H
#define AUPE_H

void err_sys(const char *msg);

void set_fl(int fd, int flags);  /* fcntl internally */
void clr_fl(int fd, int flags);

#endif