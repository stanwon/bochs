#ifndef __THREAD_SYNC_H
#define __THREAD_SYNC_H
#include "list.h"
#include "stdint.h"
#include "thread.h"

typedef struct semaphore {
  U8 value;
  ST_LIST waiters;
} ST_SEMAPHORE;

typedef struct tag_lock {
  struct tag_task_struct *holder;
  ST_SEMAPHORE semaphore;
  U32 holder_repeat_nr;
} ST_LOCK;

void lock_init(ST_LOCK *plock);
void sema_init(ST_SEMAPHORE *psema, U8 value);
void sema_down(ST_SEMAPHORE *psema);
void sema_up(ST_SEMAPHORE *psema);
void lock_acquire(ST_LOCK *plock);
void lock_release(ST_LOCK *plock);

#endif
