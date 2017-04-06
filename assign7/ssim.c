#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

#define SRAND_SEED 597226
#define NUM_PROC   10
#define MAX_MSG    (NUM_PROC - 1) * 12

int get_random_process_id();

int main() {
  int i, index, pid, status, fd[NUM_PROC][2];
  char *buffer;
  char *messages[MAX_MSG];

  // Open pipes for read / write
  for(i = 0; i < NUM_PROC; i++) {
    pipe(fd[i]);
  }

  // Fork loop. We loop NUM_PROC-1 times since we don't want
  // the last iteration to create an extra fork
  for(index = 0; index < NUM_PROC-1; index++) {
    if((pid = fork()) == -1) {
      perror("fork");
      exit(-1);
    } 
    else if(pid == 0) {
      // Child continues until next iteration to create another child proc
      // NOOP
    } else {
      // Parent should not create more child procs
      break;
    }
  }

  // Initialize pseudo-random generator
  srand(SRAND_SEED * index);

  // Close unnecessary ends of the pipe
  for(i = 0; i < NUM_PROC; i++) {
    if(i == index) {
      close(fd[i][1]);
    } else {
      close(fd[i][0]);
    }
  }

  // Write messages to random processes;
  buffer = (char *) malloc(9);
  sprintf(buffer, "process%d", index);
  for(i = 0; i < 12; i++) {
    int proc = get_random_process_id(index);
    write(fd[proc][1], buffer, 9);
  }
  free(buffer);

  // Close writing ends of processes
  for(i = 0; i < NUM_PROC; i++) {
    close(fd[i][1]);
  }

  // Allocate memory for received messages
  for(i = 0; i < MAX_MSG; i++) {
    messages[i] = (char *) malloc(9);
  }

  // Read all messages until pipe is closed
  int count = 0;
  while(1) {
    buffer = (char *) malloc(9);
    int bytes = read(fd[index][0], buffer, 9);
    
    // Read error
    if(bytes < 0) {
      printf("Proc %d = %d\n", index, fd[index][0]);
      perror("read");
      break;

    // End of file
    } else if(bytes == 0) {
      free(buffer);
      break;

    // Message received
    } else {
      memcpy(messages[count], buffer, strlen(buffer));
      count++;
      free(buffer);
    }
  }

  // Print out messages received
  for(i = 0; i < count; i++) {
    printf("process%d received a message from %s\n", index, messages[i]);
  }

  // Free memory and close out read pipe
  for(i = 0; i < MAX_MSG; i++) {
    free(messages[i]);
  }
  close(fd[index][0]);

  exit(0);
}


int get_random_process_id(int curr_proc) {
  int j;
  do {
    j = rand() % NUM_PROC;
  } while (j == curr_proc);
  return j;
}
