#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(void)
{
  int pid = fork();
  
  if(pid == 0) {
    // Child process
    printf("Child starting...\n");
    struct procinfo pi;
    
    for(int i = 0; i < 5; i++) {
      if(getprocinfo(getpid(), &pi) == 0) {
        printf("Child: state=%d, priority=%d, ticks=%d\n",
               pi.state, pi.priority, pi.ticks_used);
      }
      pause(10);  // CHANGED: sleep(10) -> pause(10)
    }
    exit(0);
  } else {
    // Parent process
    struct procinfo pi;
    
    for(int i = 0; i < 3; i++) {
      if(getprocinfo(pid, &pi) == 0) {
        printf("Monitoring child %d: state=%d, priority=%d\n",
               pid, pi.state, pi.priority);
      }
      pause(15);  // CHANGED: sleep(15) -> pause(15)
    }
    wait(0);
  }
  
  exit(0);
}
