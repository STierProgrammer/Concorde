#include "conclient_logs.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("%s: must have at least 1 argument\n", argv[0]);
        return EXIT_FAILURE;
    }
    if(initlogs(argv[1]) == -1) {
        return EXIT_FAILURE;
    }
    printf("Hello, World!\n");
    return 0;
}
