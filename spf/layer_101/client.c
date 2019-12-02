#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "system_info.h"

bool parse_int(const char* str, int* var) {
  char* end;
  *var = (int)strtol(str, &end, 10);
  if (end == str) return false;
  return true;
}

int main(int argc, char* argv[]) {
  // logging
  printf("<...initializing-client...>\n\n");
  // logging
  
  struct system_info* sys_info;
  
  // setup client from args
  int opt = 0;
  while ((opt = getopt(argc, argv, "v:m:")) != -1) {
    switch (opt) {
      case 'v':
        // logging
        printf("<...running-system-v-mode...>\n");
        // logging

        // getting system_info structure from system V memory segment
        int sysVMemID;
        parse_int(optarg, &sysVMemID);
        printf("    <...id=%u...>\n\n", sysVMemID);
        sys_info = (struct system_info*)shmat(sysVMemID, NULL, 0);
        break;
      case 'm':
        // logging
        printf("<...running-mmap-mode...>");
        //logging

        // opening file and getting system_info structure from it
				int mmapFD = open(optarg, O_RDWR, 0644);  // open file
        sys_info = (struct system_info*)mmap(NULL, sizeof(struct system_info),
            PROT_READ, MAP_SHARED, mmapFD, 0); // map file to memory
        break;
      default:
        fprintf(stderr, "Usage: %s [-v]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  // logging
  printf("<...received-data...>\n");
  printf("    <...time=%lu...>\n", (*sys_info).startup_time);
  printf("    <...pid=%u...>\n", (*sys_info).pid);
  printf("    <...uid=%u...>\n", (*sys_info).uid);
  printf("    <...gid=%u...>\n", (*sys_info).gid);
  printf("    <...ave-sys-load-1min=%f...>\n", (*sys_info).sys_loads[0]);
  printf("    <...ave-sys-load-2min=%f...>\n", (*sys_info).sys_loads[1]);
  printf("    <...ave-sys-load-5min=%f...>\n", (*sys_info).sys_loads[2]);
  printf("<...received-data...>\n\n");
  printf("<...terminating-client...>\n");
  // logging
}
