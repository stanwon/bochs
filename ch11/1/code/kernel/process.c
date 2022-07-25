#include "../include/console.h"
#include "../include/debug.h"
#include "../include/global.h"
#include "../include/interrupt.h"
#include "../include/memory.h"
#include "../include/thread.h"
#include "../include/tss.h"
#include "../include/userprog.h"
#include "../include/string.h"

extern ST_TASK_STRUCT *main_thread;
extern ST_LIST thread_ready_list;
extern ST_LIST thread_all_list;
extern void intr_exit(void);
U32 default_prio = 31;

void start_process(void *filename_) {
  void *function = filename_;
  ST_TASK_STRUCT *cur = running_thread();
  cur->self_kstack += sizeof(ST_THREAD_STACK);
  ST_INTR_STACK *proc_stack = (ST_INTR_STACK *)cur->self_kstack;
  proc_stack->edi = 0;
  proc_stack->esi = 0;
  proc_stack->ebp = 0;
  proc_stack->esp_dummy = 0;

  proc_stack->ebx = 0;
  proc_stack->edx = 0;
  proc_stack->ecx = 0;
  proc_stack->eax = 0;

  proc_stack->gs = 0;
  proc_stack->ds = SELECTOR_U_DATA;
  proc_stack->es = SELECTOR_U_DATA;
  proc_stack->fs = SELECTOR_U_DATA;

  proc_stack->eip = function;
  proc_stack->cs = SELECTOR_U_CODE;
  proc_stack->eflags = (EFLAGS_IOPL_0 | EFLAGS_MBS | EFLAGS_IF_1);
  proc_stack->esp =
      (void *)((U32)get_a_page(EN_PF_USER, USER_STACK3_VADDR) + PG_SIZE);
  proc_stack->ss = SELECTOR_U_DATA;
  asm volatile("movl %0, %%esp; jmp intr_exit" ::"g"(proc_stack) : "memory");
}

void page_dir_activate(ST_TASK_STRUCT *p_thread) {
  U32 pagedir_phy_addr = 0x100000;
  if (NULL != p_thread->pgdir) {
    pagedir_phy_addr = addr_v2p((U32)p_thread->pgdir);
  }

  asm volatile("movl %0, %%cr3" ::"r"(pagedir_phy_addr) : "memory");
}

void process_activate(ST_TASK_STRUCT *p_thread) {
  ASSERT(NULL != p_thread);
  page_dir_activate(p_thread);

  if (p_thread->pgdir) {
    update_tss_esp(p_thread);
  }
}

U32 *create_page_dir(void) {
  U32 *page_dir_vaddr = get_kernel_pages(1);
  if (page_dir_vaddr == NULL) {
    console_put_str("create_page_dir: get_kernel_pages failed!");
    return NULL;
  }
  memcpy((U32 *)((U32)page_dir_vaddr + 0x300 * 4),
         (U32 *)(0xfffff000 + 0x300 * 4), 1024);

  U32 new_page_dir_phy_addr = addr_v2p((U32)page_dir_vaddr);
  page_dir_vaddr[1023] = new_page_dir_phy_addr | PG_US_U | PG_RW_W | PG_P_1;
  return page_dir_vaddr;
}

void create_user_vaddr_bitmap(ST_TASK_STRUCT *user_prog) {
  user_prog->userprog_vaddr.vaddr_start = USER_VADDR_START;
  U32 bitmap_pg_cnt =
      DIV_ROUND_UP((0xc0000000 - USER_VADDR_START) / PG_SIZE / 8, PG_SIZE);
  user_prog->userprog_vaddr.vaddr_bitmap.bits = get_kernel_pages(bitmap_pg_cnt);
  user_prog->userprog_vaddr.vaddr_bitmap.btmp_bytes_len =
      (0xc0000000 - USER_VADDR_START) / PG_SIZE / 8;
  bitmap_init(&user_prog->userprog_vaddr.vaddr_bitmap);
}

void process_execute(void *filename, char *name) {
  ST_TASK_STRUCT *thread = get_kernel_pages(1);
  init_thread(thread, name, default_prio);
  create_user_vaddr_bitmap(thread);
  thread_create(thread, start_process, filename);
  thread->pgdir = create_page_dir();

  EN_INTR_STATUS old_status = intr_disable();
  ASSERT(!elem_find(&thread_ready_list, &thread->general_tag));
  list_append(&thread_ready_list, &thread->general_tag);

  ASSERT(!elem_find(&thread_all_list, &thread->all_list_tag));
  list_append(&thread_all_list, &thread->all_list_tag);
  intr_set_status(old_status);
}
