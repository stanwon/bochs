#include "../include/thread.h"
#include "../include/global.h"
#include "../include/memory.h"
#include "../include/string.h"

#define PG_SIZE 4096

static void kernel_thread(thread_func *function, void *func_arg) {
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
  pthread->status = EN_TASK_RUNNING;
  pthread->priority = prio;
  pthread->self_kstack = (U32 *)((U32)pthread + PG_SIZE);
  pthread->stack_magic = 0x19870916;
}

ST_TASK_STRUCT *thread_start(char *name, int prio, thread_func function,
                              void *func_arg) {
  ST_TASK_STRUCT *thread = get_kernel_pages(1);
  init_thread(thread, name, prio);
  thread_create(thread, function, func_arg);
  asm volatile("movl %0, %%esp; \
                pop %%ebp; \
                pop %%ebx; \
                pop %%edi; \
                pop %%esi; \
                ret" ::"g"(thread->self_kstack)
               : "memory");
  return thread;
}
