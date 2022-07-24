#include "../include/ioqueue.h"
#include "../include/debug.h"
#include "../include/global.h"
#include "../include/interrupt.h"

void ioqueue_init(ST_IO_QUEUE *ioq) {
  lock_init(&ioq->lock);
  ioq->producer = ioq->consumer = NULL;
  ioq->head = ioq->tail = 0;
}

static U32 next_pos(U32 pos) { return (pos + 1) % bufsize; }

bool ioq_full(ST_IO_QUEUE *ioq) {
  ASSERT(intr_get_status() == EN_INTR_OFF);
  return next_pos(ioq->head) == ioq->tail;
}

bool ioq_empty(ST_IO_QUEUE *ioq) {
  ASSERT(intr_get_status() == EN_INTR_OFF);
  return ioq->head == ioq->tail;
}

static void ioq_wait(ST_TASK_STRUCT **waiter) {
  ASSERT(*waiter == NULL && waiter != NULL);
  *waiter = running_thread();
  thread_block(EN_TASK_BLOCKED);
}

static void wakeup(ST_TASK_STRUCT **waiter) {
  ASSERT(*waiter != NULL);
  thread_unblock(*waiter);
  *waiter = NULL;
}

char ioq_getchar(ST_IO_QUEUE *ioq) {
  ASSERT(intr_get_status() == EN_INTR_OFF);

  while (ioq_empty(ioq)) {
    lock_acquire(&ioq->lock);
    ioq_wait(&ioq->consumer);
    lock_release(&ioq->lock);
  }

  char byte = ioq->buf[ioq->tail];
  ioq->tail = next_pos(ioq->tail);

  if (ioq->producer != NULL) {
    wakeup(&ioq->producer);
  }

  return byte;
}

void ioq_putchar(ST_IO_QUEUE *ioq, char byte) {
  while (ioq_full(ioq)) {
    lock_acquire(&ioq->lock);
    ioq_wait(&ioq->producer);
    lock_release(&ioq->lock);
  }

  ioq->buf[ioq->head] = byte;
  ioq->head = next_pos(ioq->head);

  if (ioq->consumer != NULL) {
    wakeup(&ioq->consumer);
  }
}
