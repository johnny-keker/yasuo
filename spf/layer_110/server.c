#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define USAGE "Usage: ./server port\n"

// ERROR HANDLING
void DIE_ON_ERROR(char *last_words) {
  if (errno) {
    fprintf(stderr, "!<...error...>!\n  %s\n  %s\n!<...error...>!\n",
        last_words, strerror(errno));
    exit(1);
  }
}
// ERROR HANDLING

// MAIN
int main(int argc, char *argv[]) {
  // PORT PARSING
  if (argc != 2) {
    fprintf(stderr, "%s", USAGE);
    exit(1);
  }
  char *port_str;
  unsigned int port = (int)strtol(argv[1], &port_str, 10);
  if (port > 65536 || *port_str != '\0' || errno != 0) {
    fprintf(stderr, "!<...error...>!\n  <...wrong-port-num...>\n!<...error...>!\n");
    exit(1);
  }
  // PORT PARSING


}
