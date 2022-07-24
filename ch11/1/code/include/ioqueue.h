#ifndef __IOQUEUE_H
#define __IOQUEUE_H

#include "stdint.h"
#include "thread.h"
#include "sync.h"

#define bufsize 64

typedef struct ioqueue{
  ST_LOCK lock;
  ST_TASK_STRUCT *producer;
  ST_TASK_STRUCT *consumer;
  char buf[bufsize];
  U32 head;
  U32 tail;
} ST_IO_QUEUE;

void ioqueue_init(ST_IO_QUEUE *ioq);
bool ioq_full(ST_IO_QUEUE *ioq);
void ioq_putchar(ST_IO_QUEUE *ioq, char byte);
bool ioq_empty(ST_IO_QUEUE *ioq);
char ioq_getchar(ST_IO_QUEUE *ioq);

#endif
