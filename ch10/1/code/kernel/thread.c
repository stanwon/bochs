#include "../include/thread.h"
#include "../include/debug.h"
#include "../include/global.h"
#include "../include/interrupt.h"
#include "../include/list.h"
#include "../include/memory.h"
#include "../include/print.h"
#include "../include/string.h"

#define PG_SIZE 4096

ST_TASK_STRUCT *main_thread;
ST_LIST thread_ready_list;
ST_LIST thread_all_list;
static ST_LIST_ELEM *thread_tag;

extern void switch_to(ST_TASK_STRUCT *cur, ST_TASK_STRUCT *nex);

// get current PCB
ST_TASK_STRUCT *running_thread() {
  U32 esp;
  asm("mov %%esp, %0" : "=g"(esp));
  return (ST_TASK_STRUCT *)(esp & 0xffffff000);
}

static void kernel_thread(thread_func *function, void *func_arg) {
  intr_enable();
  function(func_arg);
}

void thread_create(ST_TASK_STRUCT *pthread, thread_func function,
                   void *func_arg) {
  pthread->self_kstack -= sizeof(ST_INTR_STACK);
  pthread->self_kstack -= sizeof(ST_THREAD_STACK);
  ST_THREAD_STACK *kthread_stack = (ST_THREAD_STACK *)pthread->self_kstack;
  kthread_stack->eip = kernel_thread;
  kthread_stack->function = function;
  kthread_stack->func_arg = func_arg;
  kthread_stack->ebp = 0;
  kthread_stack->ebx = 0;
  kthread_stack->esi = 0;
  kthread_stack->edi = 0;
}

void init_thread(ST_TASK_STRUCT *pthread, char *name, int prio) {
  memset(pthread, 0, sizeof(*pthread));
  strcpy(pthread->name, name);

  if (pthread == main_thread) {
    pthread->status = EN_TASK_RUNNING;
  } else {
    pthread->status = EN_TASK_READY;
  }

  pthread->ticks = prio;
  pthread->elapsed_ticks = 0;
  pthread->pgdir = NULL;
  pthread->priority = prio;
  pthread->self_kstack = (U32 *)((U32)pthread + PG_SIZE);
  pthread->stack_magic = 0x19870916;
}

ST_TASK_STRUCT *thread_start(char *name, int prio, thread_func function,
                             void *func_arg) {
  ST_TASK_STRUCT *thread = get_kernel_pages(1);

  init_thread(thread, name, prio);
  thread_create(thread, function, func_arg);

  ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
  list_append(&thread_ready_list, &thread->general_tag);

  ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
  list_append(&thread_all_list, &thread->all_list_tag);

#if 0
  asm volatile("movl %0, %%esp; \
                pop %%ebp; \
                pop %%ebx; \
                pop %%edi; \
                pop %%esi; \
                ret" ::"g"(thread->self_kstack)
               : "memory");
#endif
  return thread;
}

static void make_main_thread(void) {
  main_thread = running_thread();
  init_thread(main_thread, "main", 31);

  ASSERT(!elem_find(&thread_all_list, &main_thread->all_list_tag));
  list_append(&thread_all_list, &main_thread->all_list_tag);
}

void schedule() {
  ASSERT(intr_get_status() == EN_INTR_OFF);

  ST_TASK_STRUCT *cur = running_thread();
  if (cur->status == EN_TASK_RUNNING) {
    ASSERT(!elem_find(&thread_ready_list, &cur->general_tag));
    list_append(&thread_ready_list, &cur->general_tag);
    cur->ticks = cur->priority;
    cur->status = EN_TASK_READY;
  } else {
  }

  ASSERT(!list_empty(&thread_ready_list));
  thread_tag = NULL;
  thread_tag = list_pop(&thread_ready_list);
  ST_TASK_STRUCT *next = elem2entry(ST_TASK_STRUCT, general_tag, thread_tag);
  next->status = EN_TASK_RUNNING;
  switch_to(cur, next);
}

void thread_init(void) {
  put_str("thread_init start\n");
  list_init(&thread_ready_list);
  list_init(&thread_all_list);
  make_main_thread();
  put_str("thread_init done\n");
}

void thread_block(EN_TASK_STATUS stat) {
  ASSERT((EN_TASK_BLOCKED == stat) || (EN_TASK_HANGING == stat) ||
         (EN_TASK_WAITING == stat));
  EN_INTR_STATUS old_status = intr_disable();
  ST_TASK_STRUCT *cur_thread = running_thread();
  cur_thread->status = stat;
  schedule();
  intr_set_status(old_status);
}

void thread_unblock(ST_TASK_STRUCT *pthread) {
  EN_INTR_STATUS old_status = intr_disable();
  ASSERT((EN_TASK_BLOCKED == pthread->status) ||
         (EN_TASK_HANGING == pthread->status) ||
         (EN_TASK_WAITING == pthread->status));
  if (pthread->status != EN_TASK_READY) {
    ASSERT(!elem_find(&thread_ready_list, &pthread->general_tag));
    if (elem_find(&thread_ready_list, &pthread->general_tag)) {
      PANIC("thread_unblock: blocked thread in ready list\n");
    }
    list_push(&thread_ready_list, &pthread->general_tag);
    pthread->status = EN_TASK_READY;
  }
  intr_set_status(old_status);
}
