#include "../include/timer.h"
#include "../include/debug.h"
#include "../include/io.h"
#include "../include/print.h"
#include "../include/thread.h"
#include "../include/interrupt.h"

#define IRQ0_FREQUENCY 100
#define INPUT_FREQUENCY 1193180
#define COUNTER0_VALUE INPUT_FREQUENCY / IRQ0_FREQUENCY
#define COUNTER0_PORT 0x40
#define COUNTER0_NO 0
#define COUNTER0_MODE 2
#define READ_WRITE_LATCH 3
#define PIT_CONTROL_PORT 0x43

U32 ticks;

static void frequency_set(U8 counter_port, U8 counter_no, U8 rwl,
                          U8 counter_mode, U16 counter_value) {
  outb(PIT_CONTROL_PORT, (U8)(counter_no << 6 | rwl << 4 | counter_mode << 1));
  outb(counter_port, (U8)counter_value);
  outb(counter_port, (U8)counter_value >> 8);
}

static void intr_timer_handler(void) {
  ST_TASK_STRUCT *cur_thread = running_thread();

  ASSERT(cur_thread->stack_magic == 0x19870916);

  cur_thread->elapsed_ticks++;
  ticks++;

  if (0 == cur_thread->ticks) {
    schedule();
  } else {
    cur_thread->ticks--;
  }
}

void timer_init() {
  put_str("timer_init start\n");
  frequency_set(COUNTER0_PORT, COUNTER0_NO, READ_WRITE_LATCH, COUNTER0_MODE,
                COUNTER0_VALUE);
  register_handler(0x20, intr_timer_handler);
  put_str("timer_init done\n");
}
