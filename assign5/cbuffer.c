#include "cbuffer.h"
#include <stdio.h>

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


