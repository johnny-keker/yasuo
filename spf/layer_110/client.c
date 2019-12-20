#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

#include "logger.h"
#include "error.h"

#define USAGE "./client port [directories]\n"

int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(stderr, "%s", USAGE);
    exit(1);
  }

  // PORT PARSING
  char *port_str;
  unsigned int port = (int)strtol(argv[1], &port_str, 10);
  if (port >= 65536 || *port_str != '\0' || errno != 0) {
    fprintf(stderr, "!<...error...>!\n  <...wrong-port-num...>\n!<...error...>!\n");
    exit(1);
  }
  // PORT PARSING
}
