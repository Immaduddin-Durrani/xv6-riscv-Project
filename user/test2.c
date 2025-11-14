#include "user/user.h"

int main() {
    int pid = getpid();
    printf("Test2: Yielding process PID %d starting\n", pid);

    for(int i = 0; i < 10; i++){
        printf("Test2 PID %d iteration %d: yielding...\n", pid, i);
        yield();  // Voluntarily give up CPU
    }

    printf("Test2 PID %d done\n", pid);
    exit(0);
}

