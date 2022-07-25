#include "../include/console.h"
#include "../include/debug.h"
#include "../include/init.h"
#include "../include/interrupt.h"
#include "../include/keyboard.h"
#include "../include/memory.h"
#include "../include/print.h"
#include "../include/thread.h"

void k_thread_a(void *);
void k_thread_b(void *);

int main(void) {
  put_str("This is kernel\n");
  init_all();

  thread_start("consumer_a", 31, k_thread_a, " A_");
  thread_start("consumer_b", 31, k_thread_b, " B_");

  intr_enable();
  while (1)
    ;
  return 0;
}

void k_thread_a(void *arg) {
  char *para = arg;
  while (1) {
    console_put_str(arg);
#if 0
    EN_INTR_STATUS old_status = intr_disable();

    if(!ioq_empty(&kbd_buf))
    {
      console_put_str(arg);
      char byte = ioq_getchar(&kbd_buf);
      console_put_char(byte);
    }
    intr_set_status(old_status);
#endif
  }
}

void k_thread_b(void *arg) {
  char *para = arg;
  while (1) {
    console_put_str(arg);
#if 0
    EN_INTR_STATUS old_status = intr_disable();

    if(!ioq_empty(&kbd_buf))
    {
      console_put_str(arg);
      char byte = ioq_getchar(&kbd_buf);
      console_put_char(byte);
    }
    intr_set_status(old_status);
#endif
  }
}
