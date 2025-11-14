#include "kernel/types.h"
#include "user/user.h"

int main() {
    printf("Boosting all processes...\n");
    boostproc();
    exit(0);
}
