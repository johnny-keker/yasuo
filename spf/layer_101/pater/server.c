#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "system_info.h"

#define UNDEFMODE 0
#define SYSVMODE 1
#define MMAPMODE 2
#define MSGQMODE 3

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
  int mmapFD;     // mmap file descriptor
  char filename[12];  // all numbers are representable by str[12]
  int msgQID; // message queue id
  msgbuf_t msg; // message
  // MODE SPECIFIC

  // server setup from args
  int opt = 0, run_mode = UNDEFMODE;
  while ((opt = getopt(argc, argv, "vmq")) != -1) {
    switch (opt) {
      case 'v':
        // setting run flag
        run_mode = SYSVMODE;
        // initialize system v memory segment and set system info pointer to
        // system v pointer
        sysVMemID = shmget(IPC_PRIVATE, sizeof(struct system_info), IPC_CREAT | 0644);
        void* sysVMemPointer = shmat(sysVMemID, NULL, 0);
        sys_info_ptr = (struct system_info*)sysVMemPointer;
        break;
      case 'm':
        // setting run flag
        run_mode = MMAPMODE;
        // create file and map it to memory
        sprintf(filename, "%d", pid);
        mmapFD = open(filename, O_RDWR | O_CREAT, 0644);  // create file
        ftruncate(mmapFD, sizeof(struct system_info));    // set file length
        sys_info_ptr = (struct system_info*)mmap(NULL, sizeof(struct system_info),
            PROT_WRITE | PROT_READ, MAP_SHARED, mmapFD, 0); // map file to memory
        break;
      case 'q':
        // setting run flag
        run_mode = MSGQMODE;
        // initialize system_info structure
        sys_info_ptr = (struct system_info*)malloc(sizeof(struct system_info));
        // initialize message queue
        msgQID = msgget(IPC_PRIVATE, IPC_CREAT | 0644);
        break;
      default:
        fprintf(stderr, "Usage: %s [-v] [-m] [-q]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  if (run_mode == UNDEFMODE) {
    fprintf(stderr, "Usage: %s [-v] [-m] [-q]\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  // logging
  printf("<...initializing-server...>\n\n");
  printf("<...start-parameters...>\n");
  printf("    <...pid=%u...>\n", pid);
  printf("    <...uid=%u...>\n", uid);
  printf("    <...gid=%u...>\n", gid);
  printf("<...start-parameters...>\n\n");
  // logging

  switch (run_mode) {
    case SYSVMODE:
      printf("<...running-system-v-mode...>\n");
      printf("<...created-system-v-memory-segment...>\n");
      printf("    <...id=%u...>\n", sysVMemID);
      printf("    <...size=%lu-bytes...>\n\n", sizeof(struct system_info));
      break;
    case MMAPMODE:
      printf("<...running-mmap-mode...>\n");
      printf("<...created-new-file...>\n");
      printf("    <...name=%s...>\n", filename);
      printf("    <...size=%lu-bytes...>\n\n", sizeof(struct system_info));
      break;
    case MSGQMODE:
      printf("<...running-message-queue-mode...>\n");
      printf("<...created-new-queue...>\n");
      printf("    <...id=%u...>\n", msgQID);
      break;
    default:
      fprintf(stderr, "Usage: %s [-v] [-m] [-q]\n", argv[0]);
      exit(EXIT_FAILURE);
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
    if (run_mode == MSGQMODE) {
      // receive message
      msgrcv(msgQID, &msg, 0, MSGTYPE_QUERY, 0);
      // updating current time
      (*sys_info_ptr).startup_time = time(NULL) - start_time;
      // updating average system load
      getloadavg((*sys_info_ptr).sys_loads, 3);
      // send reply
      msg.mtype = MSGTYPE_REPLY;
      memcpy(msg.mtext, sys_info_ptr, sizeof(struct system_info));
      msgsnd(msgQID, &msg, sizeof(struct system_info), 0);
    }
    else {
      // sleeping for 1 second
      sleep(1);
      // updating current time
      (*sys_info_ptr).startup_time = time(NULL) - start_time;
      // updating average system load
      getloadavg((*sys_info_ptr).sys_loads, 3);
    }
  }

  return 0;
}
