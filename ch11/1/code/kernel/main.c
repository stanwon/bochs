#include "../include/bitmap.h"
#include "../include/console.h"
#include "../include/debug.h"
#include "../include/init.h"
#include "../include/interrupt.h"
#include "../include/keyboard.h"
#include "../include/list.h"
#include "../include/memory.h"
#include "../include/print.h"
#include "../include/process.h"
#include "../include/sync.h"
#include "../include/thread.h"
#include "../include/tss.h"

void k_thread_a(void *);
void k_thread_b(void *);
void u_prog_a(void);
void u_prog_b(void);

int test_var_a = 0;
int test_var_b = 0;

int main(void) {
  put_str("This is kernel\n");
  init_all();

  thread_start("consumer_a", 31, k_thread_a, " A_");
  thread_start("consumer_b", 31, k_thread_b, " B_");

  process_execute(u_prog_a, "user_prog_a");
  process_execute(u_prog_b, "user_prog_b");
  intr_enable();
  while (1)
    ;
  return 0;
}

void k_thread_a(void *arg) {
  char *para = arg;
  while (1) {
    console_put_str("v_a:0x");
    console_put_int(test_var_a);
  }
}

void k_thread_b(void *arg) {
  char *para = arg;
  while (1) {
    console_put_str(" v_b:0x");
    console_put_int(test_var_b);
  }
}

void u_prog_a(void) {
  while (1) {
    test_var_a++;
  }
}

void u_prog_b(void) {
  while (1) {
    test_var_b++;
  }
}
