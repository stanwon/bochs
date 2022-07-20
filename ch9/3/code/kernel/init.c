#include "../include/init.h"
#include "../include/print.h"
#include "../include/interrupt.h"
#include "../include/memory.h"
#include "../include/timer.h"

void init_all()
{
  put_str("init_all\n");
  idt_init();
  mem_init();
  timer_init();
}
