#include "../include/debug.h"
#include "../include/init.h"
#include "../include/memory.h"
#include "../include/print.h"
#include "../include/thread.h"

void k_thread_a(void *);

int main(void) {
  put_str("This is kernel\n");

  init_all();

  thread_start("k_thread_a", 31, k_thread_a, "argA ");
  while (1)
    ;
  return 0;
}

void k_thread_a(void *arg)
{
  char *para = arg;
  while (1) {
    put_str(para);
  }
}
