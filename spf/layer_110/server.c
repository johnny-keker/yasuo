#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define USAGE "Usage: ./server port\n"
#define BUFSIZE 4096

// ERROR HANDLING
void DIE_ON_ERROR(char *last_words) {
  if (errno) {
    fprintf(stderr, "!<...error...>!\n  %s\n  %s\n!<...error...>!\n",
        last_words, strerror(errno));
    exit(1);
  }
}
// ERROR HANDLING

// CLIENT HANDLING LOGIC
void handle_client(int client_fd) {
  
}
// CLIENT HANDLING LOGIC

// MAIN
int main(int argc, char *argv[]) {
  // PORT PARSING
  if (argc != 2) {
    fprintf(stderr, "%s", USAGE);
    exit(1);
  }
  char *port_str;
  unsigned int port = (int)strtol(argv[1], &port_str, 10);
  if (port >= 65536 || *port_str != '\0' || errno != 0) {
    fprintf(stderr, "!<...error...>!\n  <...wrong-port-num...>\n!<...error...>!\n");
    exit(1);
  }
  // PORT PARSIN

  // SETUP SOCKET
  int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  DIE_ON_ERROR("<...cannot-create-socket...>");
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(port),
    .sin_addr = {
      .s_addr = htonl(INADDR_ANY)
    }
  };
  // SETUP SOCKET
  
  // SETUP SERVER
  bind(socket_fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
  DIE_ON_ERROR("<...cant-bind-addr-to-socket...>");
  listen(socket_fd, 10);
  DIE_ON_ERROR("<...cant-listen-on-port...>");
  // SETUP SERVER
  
  printf("\n<...listening-on-%d...>\n", port);

  // MAIN LOOP
  for (;;) {
    int client = accept(socket_fd, NULL, NULL);
    DIE_ON_ERROR("<...cant-connect-to-client...>");
    if (!fork()) {
    // HANDLE CLIENT IN CHILD PROCESS
    
    
    // HANDLE CLIENT IN CHILD PROCESS
    }
  }
  // MAIN LOOP
}
