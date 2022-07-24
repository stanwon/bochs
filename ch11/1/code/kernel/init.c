#include "../include/init.h"
#include "../include/print.h"
#include "../include/interrupt.h"
#include "../include/memory.h"
#include "../include/timer.h"
#include "../include/thread.h"
#include "../include/console.h"
#include "../include/keyboard.h"

void init_all()
{
  put_str("init_all\n");
  idt_init();
  mem_init();
  thread_init();
  timer_init();
  console_init();
  keyboard_init();
}
