#include <stdio.h>
#include <string.h>
#include "cbuffer.h"

int main() {
  cbuffer *buffer;
  buffer = cbuffer_new(64);

  char str[] = "this is a string";
  char str1[] = "this is another string";
  char str2[] = "this is the third string";
  char str5[] = "This is the last str";

  cbuffer_enqueue(buffer, str, strlen(str));
  int free = cbuffer_free(buffer);
  int size = buffer->count;
  printf("Free: %d, Size: %d\n", free, size);

  cbuffer_enqueue(buffer, str1, strlen(str1));
  free = cbuffer_free(buffer);
  size = buffer->count;
  printf("Free: %d, Size: %d\n", free, size);

  cbuffer_enqueue(buffer, str2, strlen(str2));
  free = cbuffer_free(buffer);
  size = buffer->count;
  printf("Free: %d, Size: %d\n", free, size);

  // First STR
  char *str4 = cbuffer_dequeue(buffer, strlen(str));
  printf("Dequeued: %s\n", str4);

  free = cbuffer_free(buffer);
  size = buffer->count;
  printf("Free: %d, Size: %d\n", free, size);

  // Second STR
  str4 = cbuffer_dequeue(buffer, strlen(str1));
  printf("Dequeued: %s\n", str4);

  free = cbuffer_free(buffer);
  size = buffer->count;
  printf("Free: %d, Size: %d\n", free, size);

  cbuffer_enqueue(buffer, str5, strlen(str5));
  free = cbuffer_free(buffer);
  size = buffer->count;
  int head = buffer->head;
  int tail = buffer->tail;
  printf("Free: %d, Size: %d\n", free, size);
  printf("Head: %d, Tail: %d\n", head, tail);

  // Third STR
  str4 = cbuffer_dequeue(buffer, strlen(str2));
  printf("Dequeued: %s\n", str4);

  free = cbuffer_free(buffer);
  size = buffer->count;
  printf("Free: %d, Size: %d\n", free, size);

  // Fourth STR
  str4 = cbuffer_dequeue(buffer, strlen(str5));
  printf("Dequeued: %s\n", str4);

  free = cbuffer_free(buffer);
  size = buffer->count;
  printf("Free: %d, Size: %d\n", free, size);

  return 0;
}
