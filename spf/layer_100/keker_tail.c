#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>

#define WORD_MODE_FLAG 1

const int BUFSIZE = 4096;
const char* INVALID_NUM = "Cant parse given number\n";

bool parse_int(const char* str, long* var) {
  char* end;
  *var = strtol(str, &end, 10);
  if (end == str) return false;
  return true;
}

char* get_file_contents(int fd) {
  char buffer[BUFSIZE];
  char* contents = NULL; 
  int byte_count, real_buf_size = 0;
  while ((byte_count = read(fd, buffer, BUFSIZE)) > 0) {
    contents = realloc(contents, real_buf_size + byte_count);
    memcpy(contents + real_buf_size, buffer, byte_count);
    real_buf_size += byte_count;
  }
  contents = realloc(contents, real_buf_size + 1);
  contents[real_buf_size] = '\0';
  return contents;
}

void print_last_n_lines(char* contents, int n, int flags) {
  char* end = contents + strlen(contents);
  char* curr_char;
  int curr_n = 0;
  if (*(end - 1) == '\n')
    curr_n--;
  
  if ((flags & WORD_MODE_FLAG) != 0)
    for (curr_char = end - 2; *curr_char == '\n'; --curr_char)
      curr_n--;

  for (curr_char = end; curr_char != contents; --curr_char) {
    if (*curr_char == '\n' || ((flags & WORD_MODE_FLAG) != 0 
          && (*curr_char == ' ' 
          && *(curr_char + 1) != '\n' 
          && *(curr_char - 1) != '\n'
          && *(curr_char + 1) != ' '))) 
      curr_n++;
    if (curr_n == n)
      break;
  }
  if (*curr_char == '\n' || (((flags & WORD_MODE_FLAG) != 0) && *curr_char == ' '))
    curr_char++;
  write(STDOUT_FILENO, curr_char, end - curr_char);
}

int main(int argc, char *argv[]) {
  int opt = 0;
  long num_lines = 10;
  int flags = 0;

  while ((opt = getopt(argc, argv, "n:w")) != -1) {
    switch (opt) {
      case 'n':
        if (!parse_int(optarg, &num_lines)) {
          write(STDERR_FILENO, INVALID_NUM, strlen(INVALID_NUM));
          return 1;
        }
        break;
      case 'w':
        flags |= WORD_MODE_FLAG;
        break;
      default:
        return 1;
    }
  }
  char* contents;

  if (optind != argc && *argv[optind] != '-') {
    if (optind + 1 == argc) {
      int fd = open(argv[optind], O_RDONLY);
      if (errno != 0) {
        char* error = strerror(errno);
        write(STDERR_FILENO, error, strlen(error));
        write(STDERR_FILENO, "\n", 1);
        return 1;
      }
      contents = get_file_contents(fd);
      close(fd);
      print_last_n_lines(contents, num_lines, flags);
    }
    else {
      for (int i = optind; i < argc; ++i) {
        if (*argv[i] == '-') {
          write(STDOUT_FILENO, "==> standard input <==\n", 23);
          contents = get_file_contents(STDIN_FILENO);
          print_last_n_lines(contents, num_lines, flags);
          write(STDOUT_FILENO, "\n", 1);
        }
        else {
          int fd = open(argv[i], O_RDONLY);
          if (errno != 0) {
            char* error = strerror(errno);
            write(STDERR_FILENO, error, strlen(error));
            write(STDERR_FILENO, "\n", 1);
            return 1;
          }
          write(STDOUT_FILENO, "==> ", 4);
          write(STDOUT_FILENO, argv[i], strlen(argv[i]));
          write(STDOUT_FILENO, " <==\n", 5);
          contents = get_file_contents(fd);
          close(fd);
          print_last_n_lines(contents, num_lines, flags);
          write(STDOUT_FILENO, "\n", 1);
        }
      }
    }
  }
  else {
    contents = get_file_contents(STDIN_FILENO);
    print_last_n_lines(contents, num_lines, flags);
  }
}
