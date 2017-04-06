#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h> 
#include <fcntl.h>

#define BUFF_SIZE 1024
typedef int fd;

typedef struct cbuffer {
  int head;
  int tail;
  size_t capacity;
  size_t count;
  char *container;
} cbuffer;

cbuffer*     cbuffer_new(size_t capacity);
void         cbuffer_enqueue(cbuffer*, char *, size_t count);
char*        cbuffer_dequeue(cbuffer*, size_t count);
int          cbuffer_empty(cbuffer*);
int          cbuffer_free(cbuffer*);
int          cbuffer_find(cbuffer*, char);

fd    open_input_file(char *);
void  print_usage();

int read_buffer(fd, cbuffer *);
int write_buffer(fd, cbuffer *);

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

  cbuffer *buffer1;
  cbuffer *buffer2;

  buffer1 = cbuffer_new(BUFF_SIZE);
  buffer2 = cbuffer_new(BUFF_SIZE);

  int buff1_read, buff1_write;
  int buff2_read, buff2_write;

  do {
    do {
      buff1_read = read_buffer(fdin1, buffer1);
      if(buff1_read == 0 && cbuffer_free(buffer1) != BUFF_SIZE) {
        buff1_read = buffer1->capacity - cbuffer_free(buffer1);
      }

      if(buff1_read > 0) {
        buff1_write = write_buffer(fdout, buffer1);
      }
    } while(buff1_write == BUFF_SIZE);

    do {
      buff2_read = read_buffer(fdin2, buffer2);
      if(buff2_read == 0 && cbuffer_free(buffer2) != BUFF_SIZE) {
        buff2_read = buffer2->capacity - cbuffer_free(buffer2);
      }

      if(buff2_read > 0) {
        buff2_write = write_buffer(fdout, buffer2);
      }
    } while(buff2_write == BUFF_SIZE);
  } while(buff1_read || buff2_read);

  close(fdin1);
  close(fdin2);
  close(fdout);
}

int read_buffer(fd fdin, cbuffer *buff) {
  // Check how many free bytes
  size_t free = cbuffer_free(buff);

  // Allocate a string with the length of the free memory of the buffer
  char *str = (char *) malloc(free);

  // Read n-bytes into the string from the fd
  int byte_count = protected_read(fdin, str, free);

  // Store the read bytes into the cbuffer
  cbuffer_enqueue(buff, str, byte_count);

  // Return the number of bytes read from fd
  return byte_count;
}

int write_buffer(fd fdout, cbuffer *buff) {
  // Nothing to write
  if(cbuffer_empty(buff)) {
    return 0;
  }

  // Find a newline character. -1 if not found.
  int offset = cbuffer_find(buff, '\n');

  // No newline found. Write whole buffer.
  if(offset == -1) {
    char *str = cbuffer_dequeue(buff, buff->capacity);
    protected_write(fdout, str, buff->capacity);
    return buff->capacity;
  } else {
    char *str = cbuffer_dequeue(buff, offset);
    protected_write(fdout, str, offset);
    return offset;
  }
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


ssize_t protected_read(fd fdin, char *buffer, size_t size) {
  ssize_t count;
  if((count = read(fdin, buffer, size)) < 0) {
    perror("read error\n");
    close(fd);
    exit(-1);
  }
  return count;
}

ssize_t protected_write(fd fdout, char *buffer, size_t size) {
  ssize_t count;
  if((count = write(fdout, buffer, size)) < 0) {
    perror("write error\n");
    close(fdout);
    exit(-1);
  }
  return count;
}

void print_usage() {
  write(STDERR_FILENO, "Usage: ", 8);  
  write(STDERR_FILENO, "kitkat ", 8);
  write(STDERR_FILENO, " file1 file2 [file3]\n", 23);
}

cbuffer *cbuffer_new(size_t capacity) {
  cbuffer *buff = (cbuffer *) malloc(sizeof(struct cbuffer));

  // malloc failed
  if(buff == NULL) {
    return NULL;
  }

  buff->head = 0;
  buff->tail = 0;
  buff->count = 0;
  buff->capacity = capacity;
  buff->container = (char *) malloc(buff->capacity);
  return buff;
}

void cbuffer_enqueue(cbuffer *buff, char *str, size_t count) {
  int i, tail, capacity; 
  tail = buff->tail;
  capacity = buff->capacity;

  for(i = 0; i < count; i++) {
    buff->container[(tail + i) % capacity] = str[i];
    buff->count++;
  }

  buff->tail = (tail + i) % capacity;
}

char *cbuffer_dequeue(cbuffer *buff, size_t count) {
  char *str = (char *) malloc(sizeof(char) * (count + 1));

  if(str == NULL) {
    return NULL;
  }

  int i, head, capacity;
  head = buff->head;
  capacity = buff->capacity;

  for(i = 0; i < count; i++) {
    str[i] = buff->container[(head + i) % capacity];
    buff->count--;
  }

  str[i] = '\0';
  buff->head = (head + i) % capacity;
  return str;
}

int cbuffer_empty(cbuffer *buff) {
  if(buff->tail == buff->head && buff->count == 0) {
    return 1;
  } else {
    return 0;
  }
}

int cbuffer_free(cbuffer *buff) {
  return buff->capacity - buff->count;
}

int cbuffer_find(cbuffer *buff, char c) {
  int i, count, head, capacity;
  count = buff->count;
  head = buff->head;
  capacity = buff->capacity;
  for(i = 0; i < count; i++) {
    int idx = (head + i) % capacity;
    if(buff->container[idx] == c) {
      return i + 1;
    } 
  }

  return -1;
}
