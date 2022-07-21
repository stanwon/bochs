#include "../include/debug.h"
#include "../include/init.h"
#include "../include/memory.h"
#include "../include/print.h"

int main(void) {
  put_str("This is kernel\n");
  init_all();

  void *addr = get_kernel_pages(3);
  while (1)
    ;
  put_str("get_kernel_page start vaddr is ");
  put_int((U32)addr);
  put_str("\n");

  return 0;
}
