#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int pid = getpid();
  struct procinfo pi;
  if(getprocinfo(pid, &pi) < 0){
    printf("getprocinfo failed\n");
    exit(1);
  }
  printf("pid %d state %d priority %d ticks_used %d\n",
         pi.pid, pi.state, pi.priority, pi.ticks_used);
  printf("q_ticks: %d %d %d %d\n", pi.q_ticks[0], pi.q_ticks[1], pi.q_ticks[2], pi.q_ticks[3]);
  exit(0);
}
