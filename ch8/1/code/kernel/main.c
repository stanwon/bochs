#include "../include/print.h"
#include "../include/init.h"
#include "../include/debug.h"

int main(void) {
  put_str("This is kernel\n");
  init_all();

  U32 mem_bytes_total = (*(U32 *)(0xb00));

  while (1)
    ;
  return 0;
}
