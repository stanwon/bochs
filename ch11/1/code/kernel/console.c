#include "../include/console.h"
#include "../include/print.h"
#include "../include/stdint.h"
#include "../include/memory.h"
#include "../include/sync.h"
#include "../include/thread.h"

static ST_LOCK console_lock;

void console_init() {
  lock_init(&console_lock);
}

void console_acquire() {
  lock_acquire(&console_lock);
}

void console_release() {
  lock_release(&console_lock);
}

void console_put_str(char *str) {
  console_acquire();
  put_str(str);
  console_release();
}

void console_put_char(U8 char_asci) {
  console_acquire();
  put_char(char_asci);
  console_release();
}

void console_put_int(U32 num) {
  console_acquire();
  put_int(num);
  console_release();
}
