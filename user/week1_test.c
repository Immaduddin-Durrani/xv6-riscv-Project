#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void
test_basic_info(void)
{
  printf("=== Basic Process Info Test ===\n");
  struct procinfo pi;
  if(getprocinfo(getpid(), &pi) == 0) {
    printf("Current process: pid=%d, state=%d, priority=%d\n",
           pi.pid, pi.state, pi.priority);
  }
}

void
test_fork_info(void)
{
  printf("=== Fork Process Info Test ===\n");
  int pid = fork();
  if(pid == 0) {
    struct procinfo pi;
    getprocinfo(getpid(), &pi);
    printf("Child: pid=%d, priority=%d\n", pi.pid, pi.priority);
    exit(0);
  } else {
    wait(0);
    printf("Parent: child completed\n");
  }
}

int
main(void)
{
  printf("Week 1 MLFQ Scheduler Tests\n\n");
  
  test_basic_info();
  printf("\n");
  
  test_fork_info();
  printf("\n");
  
  printf("Week 1 testing completed successfully!\n");
  printf("Ready for Week 2 MLFQ implementation.\n");
  
  exit(0);
}
