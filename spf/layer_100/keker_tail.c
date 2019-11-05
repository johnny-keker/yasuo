#include<stdio.h>
#include<unistd.h>

int main(int argc, char *argv[]) {
  int opt = 0;
  while ((opt = getopt(argc, argv, "hn")) != -1) {
    switch (opt) {
      case 'h':
        write(STDOUT_FILENO, "Hello\n", 7 * sizeof(char));
        break;
      case 'n':
        write(STDOUT_FILENO, "DIE\n", 5 * sizeof(char));
        break;
      default:
        write(STDOUT_FILENO, "WhoCares\n", 10 * sizeof(char));
        break;
    }
  }
}
