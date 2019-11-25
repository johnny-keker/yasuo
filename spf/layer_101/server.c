#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFSIZE 4096
#define SYSVMODE 1

int main(int argc, char *argv[]) {
  // storing pid, uid, gid
  pid_t pid = getpid();
  uid_t uid = getuid();
  gid_t gid = getgid();
  // storing server start time
  time_t start_time = time(NULL);
  // time since start time
  time_t current_time = 0;
  // array for getting average system load
  double loads[3];

  // MODE SPECIFIC
  int sysVMemID; // system V memory segment id
  // MODE SPECIFIC
  
  // logging
  printf("<...initializing-server...>\n\n");
  printf("<...start-parameters...>\n");
  printf("    <...pid=%u...>\n", pid);
  printf("    <...uid=%u...>\n", uid);
  printf("    <...gid=%u...>\n", gid);
  printf("<...start-parameters...>\n");
  // logging

  // server setup from args
  int run_mode = -1, opt = 0;
  while ((opt = getopt(argc, argv, "v")) != -1) {
    switch (opt) {
      case 'v':
        run_mode = SYSVMODE;
        sysVMemID = shmget(pid, BUFSIZE, IPC_CREAT | IPC_EXCL);
        break;
      default:
        fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  // server mainloop
  for (;;) {
    // sleeping for 1 second
    sleep(1);
    // updating current time
    current_time = time(NULL) - start_time;
    // updating average system load
    getloadavg(loads, 3);
  }

  return 0;
}
