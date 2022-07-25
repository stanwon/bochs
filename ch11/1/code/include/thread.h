#ifndef __THREAD_THREAD_H
#define __THREAD_THREAD_H
#include "sync.h"
#include "memory.h"
#include "stdint.h"
#include "list.h"

#define PG_SIZE 4096

typedef void thread_func(void *);

typedef enum task_status {
  EN_TASK_RUNNING,
  EN_TASK_READY,
  EN_TASK_BLOCKED,
  EN_TASK_WAITING,
  EN_TASK_HANGING,
  EN_TASK_DIED
} EN_TASK_STATUS;

typedef struct tag_intr_stack {
  U32 vec_no;
  U32 edi;
  U32 esi;
  U32 ebp;
  U32 esp_dummy;
  U32 ebx;
  U32 edx;
  U32 ecx;
  U32 eax;
  U32 gs;
  U32 fs;
  U32 es;
  U32 ds;

  U32 err_code;
  void (*eip)(void);
  U32 cs;
  U32 eflags;
  void *esp;
  U32 ss;
} ST_INTR_STACK;

typedef struct tag_thread_stack {
  U32 ebp;
  U32 ebx;
  U32 edi;
  U32 esi;

  void (*eip)(thread_func *func, void *func_arg);
  void(*unused_retaddr);
  thread_func *function;
  void *func_arg;
} ST_THREAD_STACK;

typedef struct tag_task_struct {
  U32 *self_kstack;
  EN_TASK_STATUS status;
  char name[16];
  U8 priority;
  U8 ticks;
  U32 elapsed_ticks;
  ST_LIST_ELEM general_tag;
  ST_LIST_ELEM all_list_tag;
  U32 *pgdir;
  ST_VIRTUAL_ADDR userprog_vaddr;
  U32 stack_magic;
} ST_TASK_STRUCT;

ST_TASK_STRUCT *thread_start(char *name, int prio, thread_func function, void *func_arg);
ST_TASK_STRUCT *running_thread();
void schedule();
void thread_init(void);
void thread_block(EN_TASK_STATUS stat);
void thread_unblock(ST_TASK_STRUCT *pthread);
void init_thread(ST_TASK_STRUCT *pthread, char *name, int prio);
void thread_create(ST_TASK_STRUCT *pthread, thread_func function, void *func_arg);

#endif
