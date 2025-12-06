#include "kernel/types.h"
#include "user/user.h"

#define N 3  // Smaller number for clearer output
#define ITER 50 // More iterations to ensure processes are alive during boost test

// CPU-bound: should be demoted to lower queues
void cpu_bound(int id) {
    int i, sum = 0;
    struct procinfo pi;
    
    printf("CPU-bound %d started\n", getpid());
    
    for (i = 0; i < ITER; i++) {
        // Intensive computation
        for (int j = 0; j < 5000000; j++) {
            sum += j % (i+1);
            if (j % 1000000 == 0) {
                // Check queue occasionally
                getprocinfo(getpid(), &pi);
            }
        }

        getprocinfo(getpid(), &pi);
        printf("CPU %d iter %d: queue %d ticks %d\n", 
               getpid(), i, pi.priority, pi.ticks_used);
        
        // Rarely yield to show CPU-bound nature
        if (i % 5 == 0) yield();
    }
    printf("CPU-bound %d finished\n", getpid());
    exit(0);
}

// I/O-bound: should stay in higher queues
void io_bound(int id) {
    int i;
    struct procinfo pi;
    
    printf("I/O-bound %d started\n", getpid());
    
    for (i = 0; i < ITER; i++) {
        // Frequent sleeping = I/O behavior
        sleep(5);
        
        getprocinfo(getpid(), &pi);
        printf("I/O %d iter %d: queue %d ticks %d\n", 
               getpid(), i, pi.priority, pi.ticks_used);
        
        // Frequent yielding
        yield();
    }
    printf("I/O-bound %d finished\n", getpid());
    exit(0);
}

// Mixed behavior: tests adaptability
void mixed_bound(int id) {
    int i, sum = 0;
    struct procinfo pi;
    
    printf("Mixed %d started\n", getpid());
    
    for (i = 0; i < ITER; i++) {
        if (i % 3 == 0) {
            // I/O phase
            sleep(3);
        } else {
            // CPU phase  
            for (int j = 0; j < 2000000; j++) sum += j;
        }
        
        getprocinfo(getpid(), &pi);
        printf("Mixed %d iter %d: queue %d ticks %d\n", 
               getpid(), i, pi.priority, pi.ticks_used);
        
        yield();
    }
    printf("Mixed %d finished\n", getpid());
    exit(0);
}

// State names for debugging
char* state_name(int state) {
    switch(state) {
        case 0: return "UNUSED";
        case 1: return "USED";
        case 2: return "SLEEPING";
        case 3: return "RUNNABLE";
        case 4: return "RUNNING";
        case 5: return "ZOMBIE";
        default: return "UNKNOWN";
    }
}

void test_boosting() {
    printf("\n=== Testing Priority Boost ===\n");
    
    // Check states before boost
    printf("Pre-boost state snapshot (only active processes):\n");
    for (int i = 0; i < N*3; i++) {
        struct procinfo pi;
        if (getprocinfo(i+4, &pi) == 0) { // PIDs start from 4
            // Only show non-ZOMBIE processes for meaningful boost test
            if (pi.state != 5) { // 5 = ZOMBIE
                printf("PID %d: queue %d state %s\n", pi.pid, pi.priority, state_name(pi.state));
            }
        }
    }
    
    // Perform boost
    printf("Calling boostproc()...\n");
    boostproc();
    printf("Boost completed.\n");
    
    // Check states after boost (immediately, no sleep)
    printf("Post-boost state snapshot (only active processes):\n");
    for (int i = 0; i < N*3; i++) {
        struct procinfo pi;
        if (getprocinfo(i+4, &pi) == 0) {
            if (pi.state != 5) { // 5 = ZOMBIE
                printf("PID %d: queue %d state %s\n", pi.pid, pi.priority, state_name(pi.state));
            }
        }
    }
}

int main() {
    int pids[N*3];
    
    printf("=== Comprehensive MLFQ Test ===\n");
    printf("Testing: Queue behavior, Demotion/Promotion, Starvation Prevention\n\n");

    // Create different types of processes
    for (int i = 0; i < N; i++) {
        if ((pids[i] = fork()) == 0) cpu_bound(i);
        if ((pids[N+i] = fork()) == 0) io_bound(i); 
        if ((pids[2*N+i] = fork()) == 0) mixed_bound(i);
    }

    // Let processes run for a while
    sleep(50);
    
    // Test boosting mechanism
    test_boosting();
    
    // Let processes continue after boost
    sleep(50);

    // Wait for all children
    for (int i = 0; i < N*3; i++) {
        wait(0);
    }

    printf("\n=== MLFQ Test Completed ===\n");
    exit(0);
}
