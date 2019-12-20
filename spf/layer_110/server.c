#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>

#include "logger.h"

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

// SENDING MESSAGE
void send_message(int client_fd, char* message) {
  write(client_fd, message, strlen(message));
  errno = 0;
}
// SENDING MESSAGE

// REQUEST PROCESSING
void process_request(int client_fd, char* request) {
  int dir_count = 0;
  int length = (int)strlen(request);
  for (int i = 0; i < length; i++) {
    if (request[i] == '\r') {
      request[i] = '\0';
      dir_count++;
    }
  }
  for (int i = 0; i < dir_count; i++) {
    format_string("requested-directory");
    format_string(request);
    // PROCESS CURRENT DIRECTORY
    DIR *current_dir = opendir(request);
    char *out_buf = (char*)malloc(BUFSIZE);
    sprintf(out_buf, "dir-'%s'\n", request);
    if (errno == EACCES) {
      sprintf(out_buf, "dir-'%s'\n!permission-denied!\n", request);
      send_message(client_fd, out_buf);
      continue;
    }
    else if (errno == ENOENT) {
      sprintf(out_buf, "dir-'%s'\n!not-exist!\n", request);
      send_message(client_fd, out_buf);
      continue;
    }
    send_message(client_fd, out_buf);
    struct dirent *current_entity;
    while ((current_entity = readdir(current_dir)) != NULL) {
      sprintf(out_buf, "%s\n", current_entity->d_name);
      send_message(client_fd, out_buf);
    }
    sprintf(out_buf, "\n\n");
    send_message(client_fd, out_buf);
    closedir(current_dir);
    // PROCESS CURRENT DIRECTORY
    request += strlen(request);
    request += 2;                      // skip \0/n
  }
  separate();
}
// REQUEST PROCESSING

// CLIENT HANDLING LOGIC
void handle_client(int client_fd) {
  format_string("client-%d-connected", client_fd);
  char *client_buffer = (char*)malloc(BUFSIZE);
  char *req = NULL;
  int bytes_read;
  int len = 0;
  while ((bytes_read = read(client_fd, client_buffer, BUFSIZE)) > 0) {
    if (bytes_read == 2) {                // only /r/n => empty line
      req[len - 1] = '\0';                // programmaticaly insert \0 to the end of request
      process_request(client_fd, req);
      break;
    }
    req = realloc(req, len + bytes_read);
    for (int i = 0; i < bytes_read; i++) {
      req[len + i] = client_buffer[i];
    }
    len += bytes_read;
  }
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
  listen(socket_fd, 0);
  DIE_ON_ERROR("<...cant-listen-on-port...>");
  // SETUP SERVER

  // GETTING USERNAME
  char *username = getlogin();
  DIE_ON_ERROR("<...cant-get-username...>");
  // GETTING USERNAME

  // LOGGING
  separate();
  format_string("yasuo-tcp-server");
  format_string("hello-%s", username);
  format_string("@listening-on-%d@", port);
  separate();
  // LOGGING
  
  // MAIN LOOP
  for (;;) {
    int client = accept(socket_fd, NULL, NULL);
    DIE_ON_ERROR("<...cant-connect-to-client...>");
    if (!fork()) {
      // HANDLE CLIENT IN CHILD PROCESS
      handle_client(client);
      // HANDLE CLIENT IN CHILD PROCESS
    }
  }
  // MAIN LOOP
}
