#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include "system_info.h"

int main(int argc, char* argv[]) {
	// init socket address variable
	char* socket_address = "default_socket";

  // initialize system info struct
  //struct system_info* sys_info_ptr;
  // storing pid, uid, gid
  pid_t pid = getpid();
  uid_t uid = getuid();
  gid_t gid = getgid();
  // initialize start time
  //time_t start_time = time(NULL);

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
	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, socket_address, sizeof(addr.sun_path)-1);
	bind(fd, (struct sockaddr*)&addr, sizeof(addr));

	// logging
  printf("<...initializing-server...>\n\n");
  printf("<...start-parameters...>\n");
  printf("    <...pid=%u...>\n", pid);
  printf("    <...uid=%u...>\n", uid);
  printf("    <...gid=%u...>\n", gid);
  printf("<...start-parameters...>\n\n");
	printf("<...socket-name='%s'...>\n\n", socket_address);
	// logging
}
