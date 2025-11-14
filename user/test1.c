#include "user/user.h"

int main() {
    int pid = getpid();
    printf("Test1: CPU-bound process PID %d starting\n", pid);

    for(int i = 0; i < 20; i++){
        // Busy loop to consume CPU
        for(volatile int j = 0; j < 1000000; j++);
        printf("Test1 PID %d iteration %d\n", pid, i);
    }

    printf("Test1 PID %d done\n", pid);
    exit(0);
}

