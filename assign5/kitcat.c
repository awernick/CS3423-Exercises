#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>

#define BUFF_SIZE 1024
typedef int fd;

fd    open_input_file(char *);
int   write_file(fd, fd);
void  print_usage();

ssize_t  protected_read(fd, char *, size_t);
ssize_t  protected_write(fd, char *, size_t);

int main(int argc, char *argv[]) {
  if(argc < 3) {
    print_usage();
    exit(-1);
  }

  fd fdin1, fdin2, fdout;

  // Open first arguments
  fdin1 = open_input_file(argv[1]);

  if(fdin1 < 0) {
    perror(argv[1]);
    exit(-1);
  }

  // Open second argument
  fdin2 = open_input_file(argv[2]);

  if(fdin2 < 0) {
    close(fdin1);
    perror(argv[2]);
    exit(-1);
  }

  // Check for two STDIN
  if(fdin1 == STDIN_FILENO && fdin2 == STDIN_FILENO) {
    write(STDERR_FILENO, "Error: Can't read two files from STDIN\n", 42);
    print_usage();
    exit(-1);
  }

  // Open third argument, or output to stdout
  if(argc > 3) {
    fdout = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0600);
  } else {
    fdout = STDOUT_FILENO;
  }

  if(fdout < 0) {
    close(fdin1);
    close(fdin2);
    perror(argv[3]);
    exit(-1);
  }

  if(write_file(fdin1, fdout) < 0) {
    close(fdin1);
    close(fdin2);
    close(fdout);
    exit(-1);
  }

  if(write_file(fdin2, fdout) < 0) {
    close(fdin1);
    close(fdin2);
    close(fdout);
    exit(-1);
  }

  close(fdin1);
  close(fdin2);
  close(fdout);
  exit(0);
}

fd open_input_file(char *filename) {
  int fd;
  if(strlen(filename) == 1 && filename[0] == '-') {
    fd = STDIN_FILENO;
  } else {
    fd = open(filename, O_RDONLY);
  }

  return fd;
}

// Number of bytes written
int write_file(int fdin, int fdout) {
  int rd_size, wr_size, out;
  char buffer[BUFF_SIZE];
  while((rd_size = protected_read(fdin, buffer, BUFF_SIZE)) > 0) {
    out = protected_write(fdout, buffer, rd_size);

    // Check write errors
    if(out < 0) {
      return out;
    } else {
      wr_size += rd_size;
    }
  }
  
  // Check read errors
  if(rd_size < 0) {
    return rd_size;
  }

  return wr_size;
}

ssize_t protected_read(fd fdin, char *buffer, size_t size) {
  ssize_t count;
  if((count = read(fdin, buffer, size)) < 0) {
    perror("read error\n");
  }
  return count;
}

ssize_t protected_write(fd fdout, char *buffer, size_t size) {
  ssize_t count;
  if((count = write(fdout, buffer, size)) < 0) {
    perror("write error\n");
  }
  return count;
}

void print_usage() {
  write(STDERR_FILENO, "Usage: ", 8);  
  write(STDERR_FILENO, "kitkat ", 8);
  write(STDERR_FILENO, " file1 file2 [file3]\n", 23);
}
