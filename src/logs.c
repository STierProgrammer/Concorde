#include <conclient_logs.h>
#include <stdio.h>
#include <stdlib.h>

int initlogs(char *logfile) {
    if(!logfile)
        return -1;
    if(!freopen(logfile, "r+", stdout)) {
        perror("freopen");
        exit(EXIT_FAILURE);
    }
    return 0;
}

