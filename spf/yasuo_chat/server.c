#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>

#include "error.h"
#include "logger.h"

#define BUFSIZE 512
#define USAGE "Usage: ./server port\n"

/*
typedef struct {
  int id,
  char* name,
  char* contents
} chatroom;

char *chatrooms;

void setup_chatrooms() {
  char *buffer = (char*)malloc(BUFSIZE);
  int handle = open("chatrooms.xml", O_RDONLY);
  int len = 0;
  while ((bytes_read = read(file, buffer, BUFSIZE)) {
    chatrooms = realloc(chatrooms, len + bytes_read);
  }
  fclose(file);
}
}
*/

// DEFINE CHATROOM STRUCT
typedef struct {
  int id;
  int connected;
  char* port;
  char* name;
  int clients[10];
} chatroom;
// DEFINE CHATROOM STRUCT

// DEFINE CHATROOMS
chatroom chatrooms[2] = {
  {
    .id = 0,
    .connected = 0,
    .name = "Room1",
  },
  {
    .id = 1,
    .connected = 0,
    .name = "Room2",
  }
};
// DEFINE CHATROOMS

// SENDING MESSAGE
void send_message(int client_fd, char* message) {
  write(client_fd, message, strlen(message));
  errno = 0;
}
// SENDING MESSAGE

void* handle_client(void* client_fd_void) {
  int client_fd = *(int*)client_fd_void;
  char *client_buffer = (char*)malloc(BUFSIZE);
  for (;;) {
    int bytes_read;
    while ((bytes_read = read(client_fd, client_buffer, BUFSIZE)) > 0) {
      char* chat_id_str;
      unsigned int chat_id = (int)strtol(client_buffer, &chat_id_str, 10);
      for (int i = 0; i < chatrooms[chat_id].connected; i++) {
        if (chatrooms[chat_id].clients[i] == client_fd) continue;          // do not send message to user itself
        client_buffer[bytes_read] = '\0';
        send_message(chatrooms[chat_id].clients[i], client_buffer + 2);    // skip id\0
      }
    }
  }
}

int setup_socket(char* p_str) {
  // PORT PARSING
  char *port_str;
  unsigned int port = (int)strtol(p_str, &port_str, 10);
  if (port >= 65536 || *port_str != '\0' || errno != 0) {
    fprintf(stderr, "!<...error...>!\n  <...wrong-port-num...>\n!<...error...>!\n");
    exit(1);
  }
  // PORT PARSING

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

  return socket_fd;
}

int main(int argc, char *argv[]) {
  // PORT PARSING
  if (argc != 2) {
    fprintf(stderr, "%s", USAGE);
    exit(1);
  }

  int socket_fd = setup_socket(argv[1]);

  // GETTING USERNAME
  char *username = getlogin();
  DIE_ON_ERROR("<...cant-get-username...>");
  // GETTING USERNAME

  // LOGGING
  separate();
  format_string("yasuo-chatrooms");
  format_string("hello-%s", username);
  format_string("@listening-on-%s@", argv[1]);
  separate();
  // LOGGING

  // SERVER MAIN LOOP
  for (;;) {
    int client = accept(socket_fd, NULL, NULL);
    send_message(client, "Select the chat:\n");
    char* message = (char*)malloc(BUFSIZE);
    sprintf(message, "%d: %s\n", chatrooms[0].id, chatrooms[0].name);
    write(client, message, strlen(message));
    sprintf(message, "%d: %s\n%c", chatrooms[1].id, chatrooms[1].name, '\0');
    write(client, message, strlen(message));

    char *client_buffer = (char*)malloc(2);
    read(client, client_buffer, 2);
    if (client_buffer[0] == '0') {
      chatrooms[0].clients[chatrooms[0].connected] = client;
      chatrooms[0].connected++;
      format_string("client-added-to-chat-0");
    }
    else {
      chatrooms[1].clients[chatrooms[1].connected] = client;
      chatrooms[1].connected++;
      format_string("client-added-to-chat-1");
    }

    pthread_t client_thrd;
    pthread_create(&client_thrd, NULL, handle_client, &client);
  }
  // SERVER MAIN LOOP
}
