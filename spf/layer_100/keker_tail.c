#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

const int BUFSIZE = 4096;

int parse_int(const char* str, long* var) {
  char* end;
  *var = strtol(str, &end, 10);
  return 0;
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

void print_last_n_lines(char* contents, int n) {
  char* end = contents + strlen(contents);
  char* curr_char;
  int curr_n = 0;
  if (*(end - 1) == '\n')
    curr_n--;
  for (curr_char = end; curr_char != contents; --curr_char) {
    if (*curr_char == '\n') {
      curr_n++;
      if (curr_n == n)
        break;
    }
  }
  if (*(curr_char) == '\n')
    curr_char++;
  write(STDOUT_FILENO, curr_char, end - curr_char);
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
  char* contents;

  if (optind != argc && *argv[optind] != '-') {
    if (optind == argc + 1) {
      int fd = open(argv[optind], O_RDONLY);
      contents = get_file_contents(fd);
      close(fd);
      print_last_n_lines(contents, num_lines);
    }
    else {
      for (int i = optind; i < argc; ++i) {
        if (*argv[i] == '-') {
          write(STDOUT_FILENO, "==> standard input <==\n", 23);
          contents = get_file_contents(STDIN_FILENO);
          print_last_n_lines(contents, num_lines);
          write(STDOUT_FILENO, "\n", 1);
        }
        else {
          write(STDOUT_FILENO, "==> ", 4);
          write(STDOUT_FILENO, argv[i], strlen(argv[i]));
          write(STDOUT_FILENO, "<==\n", 4);
          int fd = open(argv[i], O_RDONLY);
          contents = get_file_contents(fd);
          close(fd);
          print_last_n_lines(contents, num_lines);
          write(STDOUT_FILENO, "\n", 1);
        }
      }
    }
  }
  else {
    contents = get_file_contents(STDIN_FILENO);
    print_last_n_lines(contents, num_lines);
  }
}
