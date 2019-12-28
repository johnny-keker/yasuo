#include <stdio.h>
#include <pthread.h>
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

int get_socket(char *host, char* p_str) {
  // SETUP SOCKET
  struct addrinfo hints = {
    .ai_family = AF_INET, .ai_socktype = SOCK_STREAM, .ai_protocol = IPPROTO_TCP
  };
  struct addrinfo *addr;
  int errcode;
  if ((errcode = getaddrinfo(host, p_str, &hints, &addr)) != 0) {
    fprintf(stderr, "!<...error...>!\n  %s\n!<...error...>!\n", gai_strerror(errcode));
    exit(1);
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
    fprintf(stderr, "!<...error...>!\n  <...'%s:%s'-unreachable...>\n!<...error...>!\n", host, p_str);
    exit(1);
  }
  freeaddrinfo(addr);
  // FINDING CORRECT ADDRESS FROM getaddinfo OUTPUT
  return socket_fd;
}

unsigned int try_get_port(char *p_str) {
  // PORT PARSING
  char *port_str;
  unsigned int port = (int)strtol(p_str, &port_str, 10);
  if (port >= 65536 || *port_str != '\0' || errno != 0) {
    fprintf(stderr, "!<...error...>!\n  <...wrong-port-num...>\n!<...error...>!\n");
    exit(1);
  }
  // PORT PARSING
  return port;
}

void *listener(void *socket_fd_ptr) {
  int socket_fd = *(int*)socket_fd_ptr;
  char *buf = (char*)malloc(BUFSIZE);
  int bytes_read;
  for (;;) {
    while ((bytes_read = read(socket_fd, buf, BUFSIZE)) > 0) {
      buf[bytes_read] = '\0';
      write(STDOUT_FILENO, buf, bytes_read);
    }
  }
}

int main(int argc, char *argv[]) {
  // CHECK ARGC
  if (argc != 3) {
    fprintf(stderr, "%s", USAGE);
    exit(1);
  }
  // CHECK ARGC

  //unsigned int port = try_get_port(argv[2]);
  int socket_fd = get_socket(argv[1], argv[2]);

  char* nickname = (char*)malloc(BUFSIZE);
  printf("Enter nickname:\n");
  scanf("%s", nickname);

  // CHAT SELECTION
  char *buf = (char*)malloc(BUFSIZE);
  int bytes_read;
  while ((bytes_read = read(socket_fd, buf, BUFSIZE)) > 0) {
    write(STDOUT_FILENO, buf, bytes_read);
    if (buf[bytes_read] == '\0') break;
  }
  // CHAT SELECTION

  // SENDING CHAT ID
  char* chat_id = (char*)malloc(1);
  scanf("%s", chat_id);
  write(socket_fd, chat_id, 2);
  DIE_ON_ERROR("<cant-send-message>");
  // SENDING CHAT ID
  
  char *message = (char*) malloc(BUFSIZE);
  sprintf(message, "%s has joined the chat!\n%c", nickname, '\0');
  write(socket_fd, message, strlen(message));

  pthread_t listen_thrd;
  pthread_create(&listen_thrd, NULL, listener, &socket_fd);
  buf[0] = '\0';
  for (;;) {
    char* message = (char*)malloc(BUFSIZE);
    size_t bufsize = BUFSIZE;
    getline(&message, &bufsize, stdin);
    if (message[0] == '\n') continue;
    sprintf(buf, "%s: %s", nickname, message);
    write(socket_fd, buf, strlen(buf));
  }
}
