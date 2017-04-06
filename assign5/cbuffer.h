#ifndef cbuffer_h
#define cbuffer_h

#include <stdlib.h>

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

#endif
