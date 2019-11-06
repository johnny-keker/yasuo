#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int parse_int(const char* str, long* var) {
  char* end;
  *var = strtol(str, &end, 10);
  return 0;
}

int main(int argc, char *argv[]) {
  int opt = 0;
  long num_lines = 0;

  while ((opt = getopt(argc, argv, "n:")) != -1) {
    switch (opt) {
      case 'n':
        parse_int(optarg, &num_lines);
        break;
    }
  }
}
