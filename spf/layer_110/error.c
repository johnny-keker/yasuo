#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

void DIE_ON_ERROR(char *last_words) {
  if (errno) {
    fprintf(stderr, "!<...error...>!\n  %s\n  %s\n!<...error...>!\n",
        last_words, strerror(errno));
    exit(1);
  }
}
