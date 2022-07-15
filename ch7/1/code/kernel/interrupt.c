#include "../include/interrupt.h"
#include "../include/global.h"
#include "../include/io.h"
#include "../include/print.h"

#define PIC_M_CTRL 0x20
#define PIC_M_DATA 0x21
#define PIC_S_CTRL 0xa0
#define PIC_S_DATA 0xa1

#define IDT_DES_CNT 0x21

typedef struct gate_desc {
  U16 func_offset_low_word;
  U16 selector;
  U8 dcount;
  U8 attribute;
  U16 func_offset_high_word;
} ST_GATE_DESC;

static void make_idt_desc(ST_GATE_DESC *p_gdesc, U8 attr,
                          intr_handler function);
static ST_GATE_DESC idt[IDT_DES_CNT];

extern intr_handler intr_entry_table[IDT_DES_CNT];

static void pic_init(void) {
  outb(PIC_M_CTRL, 0x11);
  outb(PIC_M_DATA, 0x20);

  outb(PIC_M_CTRL, 0x04);
  outb(PIC_M_DATA, 0x01);

  outb(PIC_S_CTRL, 0x11);
  outb(PIC_S_DATA, 0x28);

  outb(PIC_S_CTRL, 0x02);
  outb(PIC_S_DATA, 0x01);

  outb(PIC_M_DATA, 0xfe);
  outb(PIC_S_DATA, 0xff);

  put_str("pic_init done\n");
}

static void make_idt_desc(ST_GATE_DESC *p_gdesc, U8 attr,
                          intr_handler function) {
  p_gdesc->func_offset_low_word = (U32)function & 0x0000FFFF;
  p_gdesc->func_offset_high_word = ((U32)function & 0xFFFF0000) >> 16;
  p_gdesc->attribute = attr;
  p_gdesc->dcount = 0;
  p_gdesc->selector = SELECTOR_K_CODE;
}

static void idt_desc_init(void) {
  int i;
  for (i = 0; i < IDT_DES_CNT; i++) {
    make_idt_desc(&idt[i], IDT_DESC_DPL0, intr_entry_table[i]);
  }
  put_str("idt_desc_init donE\n");
}

void idt_init() {
  put_str("idt_init start\n");
  idt_desc_init();
  pic_init();

  U64 idt_operand = ((sizeof(idt) - 1) | ((U64)((U32)idt << 16)));
  asm volatile("lidt %0" ::"m"(idt_operand));
  put_str("idt_init done\n");
}
