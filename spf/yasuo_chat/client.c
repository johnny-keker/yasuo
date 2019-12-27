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

#define USAGE "./client host port\n"
#define BUFSIZE 4096

int main(int argc, char *argv[]) {
  // CHECK ARGC
  if (argc != 3) {
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

  char* nickname = (char*)malloc(BUFSIZE);
  printf("Enter nickname:\n");
  scanf("%s", nickname);

  // RECEIVING RESULT
  char *buf = (char*)malloc(BUFSIZE);
  int bytes_read;
  while ((bytes_read = read(socket_fd, buf, BUFSIZE)) > 0) {
    write(STDOUT_FILENO, buf, bytes_read);
    if (buf[bytes_read] == '\0') break;
  }
  // RECEIVING RESULT

  // SENDING REQUEST
  char* char_id = (char*)malloc(1);
  scanf("%s", char_id);
  write(socket_fd, char_id, 2);
  DIE_ON_ERROR("h");
  // SENDING REQUEST
}
