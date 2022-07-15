#include "../include/print.h"
#include "../include/init.h"

int main(void) {
  put_str("This is kernel\n");
  init_all();
  asm volatile("sti");
  while (1)
    ;
  return 0;
}
