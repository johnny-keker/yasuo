#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BUFSIZE 4096
#define SYSVMODE 1

// structure that contain
// required info
struct system_info {
  pid_t pid;
  uid_t uid;
  gid_t gid;
  time_t startup_time;
  double sys_loads[3];
};

int main(int argc, char* argv[]) {
  // initialize system info struct
  struct system_info* sys_info_ptr;
  // storing pid, uid, gid
  pid_t pid = getpid();
  uid_t uid = getuid();
  gid_t gid = getgid();
  // initialize start time
  time_t start_time = time(NULL);

  // MODE SPECIFIC
  int sysVMemID; // system V memory segment id
  // MODE SPECIFIC
  
  
  
  // logging
  printf("<...initializing-server...>\n\n");
  printf("<...start-parameters...>\n");
  printf("    <...pid=%u...>\n", pid);
  printf("    <...uid=%u...>\n", uid);
  printf("    <...gid=%u...>\n", gid);
  printf("<...start-parameters...>\n\n");
  // logging

  // server setup from args
  int run_mode = -1, opt = 0;
  while ((opt = getopt(argc, argv, "v")) != -1) {
    switch (opt) {
      case 'v':
        // setting run flag
        run_mode = SYSVMODE;
        // initialize system v memory segment and set system info pointer to
        // system v pointer
        sysVMemID = shmget(IPC_PRIVATE, sizeof(struct system_info), IPC_CREAT | 0644);
        void* sysVMemPointer = shmat(sysVMemID, NULL, 0);
        sys_info_ptr = (struct system_info*)sysVMemPointer;
        // logging
        printf("<...running-system-v-mode...>\n");
        printf("<...created-system-v-memory-segment...>\n");
        printf("    <...id=%u...>\n", sysVMemID);
        printf("    <...size=%u-bytes...>\n\n", sizeof(struct system_info));
        // logging
        break;
      default:
        fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  
  // initialize sys_info struct
  (*sys_info_ptr).pid = pid;
  (*sys_info_ptr).uid = uid;
  (*sys_info_ptr).gid = gid;
  (*sys_info_ptr).startup_time = time(NULL) - start_time;
  getloadavg((*sys_info_ptr).sys_loads, 3);

  // logging
  printf("<...server-is-running...>\n");
  // logging

  // server mainloop
  for (;;) {
    // sleeping for 1 second
    sleep(1);
    // updating current time
    (*sys_info_ptr).startup_time = time(NULL) - start_time;
    // updating average system load
    getloadavg((*sys_info_ptr).sys_loads, 3);
  }

  return 0;
}
