#include <stdio.h>
#include <string.h>

#define WIDTH 41

void format_string(char *string) {
  int len = strlen(string);
  int pad = ((WIDTH - len) / 2) - 2;      // - 2 because of '*' and '<' or '>' chars
  printf("*");
  for (int i = 0; i < pad; i++) {
    putchar('.');
  }
  printf("<%s>", string);
  if (len % 2 == 0)
    pad++;
  for (int i = 0; i < pad; i++) {
    putchar('.');
  }
  printf("*\n");
}

void separate() {
  for (int i = 0; i < WIDTH; i++) {
    putchar('*');
  }
  putchar('\n');
}

int main() {
  separate();
  format_string("hello-world");
  format_string("yasuo");
  format_string("this-is-logger");
  separate();
}
