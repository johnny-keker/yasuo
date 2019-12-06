#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "system_info.h"

int main(int argc, char* argv[]) {
  // define socket address variable
  char* socket_address;

  // initialize system info struct
  struct system_info* sys_info_ptr = (struct system_info*)malloc(sizeof(struct system_info));
  // storing pid, uid, gid
  pid_t pid = getpid();
  uid_t uid = getuid();
  gid_t gid = getgid();
  // initialize start time
  time_t start_time = time(NULL);

  // parse socket address from cmd args
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
  // initialize socket descriptor
  int fd = socket(AF_UNIX, SOCK_STREAM, 0);

  // initialize socket address struct
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, socket_address, sizeof(addr.sun_path)-1);
  unsigned int addr_len = sizeof(struct sockaddr_un);
  bind(fd, (const struct sockaddr*)&addr, addr_len);
  listen(fd, 0);

  // logging
  printf("<...initializing-server...>\n\n");
  printf("<...start-parameters...>\n");
  printf("    <...pid=%u...>\n", pid);
  printf("    <...uid=%u...>\n", uid);
  printf("    <...gid=%u...>\n", gid);
  printf("<...start-parameters...>\n\n");
  printf("<...socket-name='%s'...>\n\n", socket_address);
  // logging

  // initialize system info struct
  (*sys_info_ptr).pid = pid;
  (*sys_info_ptr).uid = uid;
  (*sys_info_ptr).gid = gid;
  (*sys_info_ptr).startup_time = time(NULL) - start_time;
  getloadavg((*sys_info_ptr).sys_loads, 3);

  // logging
  printf("<...server-is-running...>\n");
  // logging

  // server main loop
  for (;;) {
    // accept client connection
    int cltfd = accept(fd, (struct sockaddr*) &addr, &addr_len);
    // updating current time
    (*sys_info_ptr).startup_time = time(NULL) - start_time;
    // updating average system load
    getloadavg((*sys_info_ptr).sys_loads, 3);
    // send reply
    write(cltfd, (const void*) sys_info_ptr, sizeof(struct system_info));
    // cloase connection
    close(cltfd);
  }
}
