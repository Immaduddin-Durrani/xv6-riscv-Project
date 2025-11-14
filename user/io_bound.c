#include "kernel/types.h"
#include "user/user.h"

int main() {
    int i = 0;
    while(1) {
        i++;
        sleep(1);  // Yield frequently: I/O-bound
    }
    exit(0);
}
