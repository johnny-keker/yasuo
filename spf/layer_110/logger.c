#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include "logger.h"

void format_string(const char *fmt, ...) {
  va_list ap;
  // determine required size
  int size = 0;
  char *p = NULL;
  va_start(ap, fmt);
  size = vsnprintf(p, size, fmt, ap);
  va_end(ap);
  // .......................
  // calculate padding
  int pad = ((WIDTH - size) / 2) - 2;   // - 2 because of '*' and '<' or '>' chars
  // .................
  printf("*");                          // left border
  // insert left padding
  for (int i = 0; i < pad; i++) {
    putchar('.');
  }
  // ...................
  // print string
  p = malloc(size + 1);                 // \0
  va_start(ap, fmt);
  vsnprintf(p, size + 1, fmt, ap);
  va_end(ap);
  printf("<%s>", p);
  // ............
  // correct padding if needed
  if (size % 2 == 0)
    pad++;
  // .........................
  // insert right padding
  for (int i = 0; i < pad; i++) {
    putchar('.');
  }
  // ....................
  printf("*\n");                        // right border
}

void separate() {
  for (int i = 0; i < WIDTH; i++) {
    putchar('*');
  }
  putchar('\n');
}
