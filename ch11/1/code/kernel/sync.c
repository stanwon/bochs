#include "../include/sync.h"
#include "../include/debug.h"
#include "../include/interrupt.h"
#include "../include/thread.h"

void sema_init(ST_SEMAPHORE *psema, U8 value) {
  psema->value = value;
  list_init(&psema->waiters);
}

void lock_init(ST_LOCK *plock) {
  plock->holder = NULL;
  plock->holder_repeat_nr = 0;
  sema_init(&plock->semaphore, 1);
}

void sema_down(ST_SEMAPHORE *psema) {
  EN_INTR_STATUS old_status = intr_disable();
  while (0 == psema->value) {
    ASSERT(!elem_find(&psema->waiters, &running_thread()->general_tag));
    if (elem_find(&psema->waiters, &running_thread()->general_tag)) {
      PANIC("sema_down: thread blocked has been in waiters_list\n");
    }
    list_append(&psema->waiters, &running_thread()->general_tag);
    thread_block(EN_TASK_BLOCKED);
  }
  psema->value--;
  ASSERT(psema->value == 0);
  intr_set_status(old_status);
}

void sema_up(ST_SEMAPHORE *psema) {
  EN_INTR_STATUS old_status = intr_disable();
  ASSERT(0 == psema->value);
  if (!list_empty(&psema->waiters)) {
    ST_TASK_STRUCT *thread_blocked =
        elem2entry(ST_TASK_STRUCT, general_tag, list_pop(&psema->waiters));
    thread_unblock(thread_blocked);
  }
  psema->value++;
  ASSERT(1 == psema->value);
  intr_set_status(old_status);
}

void lock_acquire(ST_LOCK *plock) {
  if (plock->holder != running_thread()) {
    sema_down(&plock->semaphore);
    plock->holder = running_thread();
    ASSERT(0 == plock->holder_repeat_nr);
    plock->holder_repeat_nr = 1;
  } else {
    plock->holder_repeat_nr++;
  }
}

void lock_release(ST_LOCK *plock) {
  ASSERT(plock->holder == running_thread());
  if(plock->holder_repeat_nr > 1) {
    plock->holder_repeat_nr--;
    return;
  }
  ASSERT(plock->holder_repeat_nr == 1);

  plock->holder = NULL;
  plock->holder_repeat_nr = 0;
  sema_up(&plock->semaphore);
}
