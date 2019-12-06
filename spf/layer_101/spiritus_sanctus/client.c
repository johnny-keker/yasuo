#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "system_info.h"

int main(int argc, char* argv[]) {
  // define socket address
  char* socket_address;

  // initialize system info struct
  struct system_info sys_info;

  int opt = 0;
  while ((opt = getopt(argc, argv, "s:")) != -1) {
    switch (opt) {
      case 's':
        socket_address = optarg;
        break;
      default:
        fprintf(stderr, "Usage: %s [-s] [SOCKET_ADDR]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }

  // logging
  printf("<...initializing-client...>\n\n");
  printf("<...connecting-to-'%s'...>\n\n", socket_address);
  // logging

  // initialize socket descriptor
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);

  // initialize socket address struct
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_address, sizeof(addr.sun_path)-1);
  unsigned int addr_len = sizeof(struct sockaddr_un);

  // connecting to server
  connect(fd, (const struct sockaddr *) &addr, addr_len);

  // reading data
  read(fd, &sys_info, sizeof(struct system_info));

  // logging
  printf("<...received-data...>\n");
  printf("    <...time=%lu...>\n", sys_info.startup_time);
  printf("    <...pid=%u...>\n", sys_info.pid);
  printf("    <...uid=%u...>\n", sys_info.uid);
  printf("    <...gid=%u...>\n", sys_info.gid);
  printf("    <...ave-sys-load-1min=%f...>\n", sys_info.sys_loads[0]);
  printf("    <...ave-sys-load-2min=%f...>\n", sys_info.sys_loads[1]);
  printf("    <...ave-sys-load-5min=%f...>\n", sys_info.sys_loads[2]);
  printf("<...received-data...>\n\n");
  printf("<...terminating-client...>\n");
  // logging
  // closing connection
  close(fd);

  return 0;
}
