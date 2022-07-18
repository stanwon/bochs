#include "../include/print.h"
#include "../include/init.h"
#include "../include/debug.h"

int main(void) {
  put_str("This is kernel\n");
  init_all();
  ASSERT(1==2);

  while (1)
    ;
  return 0;
}
