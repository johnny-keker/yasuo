#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include "logger.h"
#include "error.h"

#define USAGE "./client host port [directories]\n"
#define BUFSIZE 4096

int main(int argc, char *argv[]) {
  // CHECK ARGC
  if (argc < 3) {
    fprintf(stderr, "%s", USAGE);
    exit(1);
  }
  // CHECK ARGC

  // PORT PARSING
  char *port_str;
  unsigned int port = (int)strtol(argv[2], &port_str, 10);
  if (port >= 65536 || *port_str != '\0' || errno != 0) {
    fprintf(stderr, "!<...error...>!\n  <...wrong-port-num...>\n!<...error...>!\n");
    exit(1);
  }
  // PORT PARSING
  
  // SETUP SOCKET
  struct addrinfo hints = {
    .ai_family = AF_INET, .ai_socktype = SOCK_STREAM, .ai_protocol = IPPROTO_TCP
  };
  struct addrinfo *addr;
  int errcode;
  if ((errcode = getaddrinfo(argv[1], argv[2], &hints, &addr)) != 0) {
    fprintf(stderr, "!<...error...>!\n  %s\n!<...error...>!\n", gai_strerror(errcode));
    return 1;
  }
  // SETUP SOCKET

  // FINDING CORRECT ADDRESS FROM getaddinfo OUTPUT
  int socket_fd;
  for (struct addrinfo* a = addr; a != NULL; a = a->ai_next) {
    if ((socket_fd = socket(a->ai_family, a->ai_socktype, a->ai_protocol)) == -1) continue;
    if (connect(socket_fd, a->ai_addr, a->ai_addrlen) == 0) break;
    socket_fd = close(socket_fd);
  }
  if (socket_fd <= 0) {
    fprintf(stderr, "!<...error...>!\n  <...'%s:%s'-unreachable...>\n!<...error...>!\n", argv[1], argv[2]);
    return 1;
  }
  freeaddrinfo(addr);
  // FINDING CORRECT ADDRESS FROM getaddinfo OUTPUT
  
  // MAKING REQUEST
  char *out_buf = (char*)malloc(BUFSIZE);
  for (int i = 3; i < argc; i++) {
    sprintf(out_buf, "%s\r\n", argv[i]);
    write(socket_fd, out_buf, strlen(argv[i]) + 2);
    DIE_ON_ERROR("<...cannot-send-message...>");
  }
  write(socket_fd, "\r\n", 2);
  DIE_ON_ERROR("<...cannot-send-message...>");
  // MAKING REQUEST

  // RECEIVING RESULT
  char *buf = (char*)malloc(BUFSIZE);
  int bytes_read;
  while ((bytes_read = read(socket_fd, buf, BUFSIZE)) > 0) {
    if (buf[bytes_read - 1] == '\n'
        && buf[bytes_read - 2] == '\r') break;
    write(STDOUT_FILENO, buf, bytes_read);
  }
  // RECEIVING RESULT
  
  close(socket_fd);
  return 0;
}
