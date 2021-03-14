#include "aupe.h"
#include <signal.h>
#include <stdio.h>


int main() {
    // code here does a different thing as required
    // but both are trivial.
    char buffer[SIG_NAME_MAXLEN];
    sig2str(SIGUSR1, buffer);
    printf("SIGUSR1: %s\n", buffer);
    sig2str(SIGINT, buffer);
    printf("SIGINT: %s\n", buffer);
}
