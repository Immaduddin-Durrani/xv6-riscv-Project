#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
child_process(int id)
{
  int pid = getpid();
  struct procinfo pi;
  
  if(getprocinfo(pid, &pi) < 0){
    printf("Child %d: getprocinfo failed\n", id);
    exit(1);
  }
  
  printf("Child %d: pid=%d, state=%d, priority=%d\n", 
         id, pi.pid, pi.state, pi.priority);
  
  if(id == 1) {
    // CPU-intensive child
    volatile int i;
    for(i = 0; i < 1000000; i++);
  } else {
    // I/O child - use pause instead of sleep
    pause(10);  // CHANGED: sleep(10) -> pause(10)
  }
  
  exit(0);
}

int
main(void)
{
  printf("Parent PID: %d\n", getpid());
  
  // Test parent process info
  struct procinfo pi;
  if(getprocinfo(getpid(), &pi) == 0) {
    printf("Parent: state=%d, priority=%d, ticks_used=%d\n",
           pi.state, pi.priority, pi.ticks_used);
  }
  
  // Create multiple children
  for(int i = 0; i < 3; i++) {
    int pid = fork();
    if(pid == 0) {
      child_process(i);
    } else if(pid < 0) {
      printf("Fork failed\n");
      exit(1);
    }
    wait(0);
  }
  
  exit(0);
}
