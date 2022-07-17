#include "../include/print.h"
#include "../include/init.h"
#include "../include/debug.h"

int main(void) {
  put_str("This is kernel\n");
  init_all();
//  ASSERT(1==2);
  put_str("filename: ");put_str(__FILE_NAME__);put_str("\n");
  while (1)
    ;
  return 0;
}
