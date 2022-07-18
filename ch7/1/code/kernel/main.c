#include "../include/init.h"
#include "../include/print.h"

int main(void) {
  put_str("This is kernel\n");
  init_all();
  put_str("hello\n");
  //asm volatile("sti");
  while (1) {
    ;
  }
  return 0;
}
