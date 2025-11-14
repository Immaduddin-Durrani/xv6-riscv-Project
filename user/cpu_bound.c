#include "kernel/types.h"
#include "user/user.h"

int main() {
    int i = 0;
    while(1) {
        i++;  // Busy loop: CPU-bound
    }
    exit(0);
}
