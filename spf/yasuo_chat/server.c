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

// DEFINE STRUCTS
typedef struct {
  int connected;
  char* name;
  int clients[10];
} chatroom;

typedef struct {
  int fd;
  chatroom *chat_info;
} client_info;
// DEFINE STRUCTS

// DEFINE CHATROOMS
chatroom chatrooms[10];
// DEFINE CHATROOMS

// SENDING MESSAGE
void send_message(int client_fd, char* message) {
  write(client_fd, message, strlen(message));
  errno = 0;
}
// SENDING MESSAGE

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

void *client_thread(void *client_info_ptr) {
  client_info *client = (client_info *)client_info_ptr;
  char *client_buffer = (char*)malloc(BUFSIZE);
  int bytes_read;
  for (;;) {
    while ((bytes_read = read(client->fd, client_buffer, BUFSIZE)) > 0) {
      client_buffer[bytes_read] = '\0';
      for (int i = 0; i < client->chat_info->connected; i++) {
        if (client->chat_info->clients[i] == client->fd) continue;          // do not send message to user itself
        send_message(client->chat_info->clients[i], client_buffer);
      }
    }
  }
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
  // LOGGING
  
  // SETTING UP CHATROOMS
  int current_room_idx = 0;
  char* room_buf = (char*)malloc(BUFSIZE);
  for (;;) {
    format_string("enter-room-name");
    scanf("%s", room_buf);
    if (strlen(room_buf) == 3 && room_buf[0] == 'e' && room_buf[1] == 'n' && room_buf[2] == 'd')
      break;
    chatrooms[current_room_idx] = *(chatroom*)malloc(sizeof(chatroom));
    chatrooms[current_room_idx].name = (char*)malloc(strlen(room_buf));
    strcpy(chatrooms[current_room_idx].name, room_buf);
    chatrooms[current_room_idx].connected = 0;
    current_room_idx++;
  }
  format_string("setup-%d-rooms", current_room_idx);
  // SETTING UP CHATROOMS
  
  format_string("@listening-on-%s@", argv[1]);
  separate();

  // SERVER MAIN LOOP
  for (;;) {
    int client_fd = accept(socket_fd, NULL, NULL);
    send_message(client_fd, "Select the chat:\n");
    char* message = (char*)malloc(BUFSIZE);
    for (int i = 0; i < current_room_idx; ++i) {
      sprintf(message, "%d: %s\n", i, chatrooms[i].name);
      write(client_fd, message, strlen(message));
    }
    sprintf(message, "%c", '\0');
    write(client_fd, message, strlen(message));

    client_info *client = (client_info*)malloc(sizeof(client_info));
    client->fd = client_fd;

    char *client_buffer = (char*)malloc(2);
    read(client_fd, client_buffer, 2);
    int room_id = atoi(client_buffer);
    chatrooms[room_id].clients[chatrooms[room_id].connected] = client_fd;
    chatrooms[room_id].connected++;
    client->chat_info = &chatrooms[room_id];
    format_string("client-added-to-chat-'%s'", chatrooms[room_id].name);
    pthread_t thrd;
    pthread_create(&thrd, NULL, client_thread, client);
  }
  // SERVER MAIN LOOP
}
