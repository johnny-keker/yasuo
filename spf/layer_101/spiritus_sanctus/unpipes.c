#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zconf.h>

#define BUFSIZE 4096

void process_file(int fd, int pipefd) {
  char inbuf[BUFSIZE];
  char pipebuf[BUFSIZE / 2];

  int bytes_read;
  while ((bytes_read = read(fd, inbuf, BUFSIZE)) > 0) {
    int j = 0;
    for (int i = 0; i < BUFSIZE; i += 2) {
      pipebuf[j] = inbuf[i];
      j++;
    }
    write(pipefd, pipebuf, bytes_read / 2);
  }
  close(pipefd);
}

int main(int argc, char** argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s file\n", argv[0]);
    return 1;
  }

  int fd;
  fd = open(argv[1], O_RDONLY);

  int pipefd[2];
  pipe(pipefd);
  int pipefd_rd = pipefd[0];
  int pipefd_wr = pipefd[1];

  int wc_pid = fork();
  if (wc_pid == 0) {
    close(pipefd_wr);
    dup2(pipefd_rd, STDIN_FILENO);
    execlp("wc", "wc", "-c", (char*)NULL);
  }
  else {
    close(pipefd_rd);
    process_file(fd, pipefd_wr);
  }
  return 0;
}
