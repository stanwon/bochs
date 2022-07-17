#include "../include/interrupt.h"
#include "../include/global.h"
#include "../include/io.h"
#include "../include/print.h"

#define PIC_M_CTRL 0x20 //主片的控制端口是0x20
#define PIC_M_DATA 0x21 //主片的数据端口是0x21
#define PIC_S_CTRL 0xa0 //从片的控制端口是0xa0
#define PIC_S_DATA 0xa1 //从片的数据端口是0xa1

#define IDT_DESC_CNT 0x21

#define EFLAGS_IF 0x00000200
#define GET_FLAGS(EFLAG_VAR) asm volatile("pushfl; popl %0" : "=g"(EFLAG_VAR))

typedef struct gate_desc {
  U16 func_offset_low_word;
  U16 selector;
  U8 dcount;
  U8 attribute;
  U16 func_offset_high_word;
} ST_GATE_DESC;

static void make_idt_desc(ST_GATE_DESC *p_gdesc, U8 attr,
                          intr_handler function);
static ST_GATE_DESC idt[IDT_DESC_CNT];

char *intr_name[IDT_DESC_CNT];
intr_handler idt_table[IDT_DESC_CNT];
extern intr_handler intr_entry_table[IDT_DESC_CNT];

static void pic_init(void) {
  outb(PIC_M_CTRL, 0x11);
  outb(PIC_M_DATA, 0x20);

  outb(PIC_M_DATA, 0x04);
  outb(PIC_M_DATA, 0x01);

  outb(PIC_S_CTRL, 0x11);
  outb(PIC_S_DATA, 0x28);

  outb(PIC_S_DATA, 0x02);
  outb(PIC_S_DATA, 0x01);

  outb(PIC_M_DATA, 0xfe);
  outb(PIC_S_DATA, 0xff);

  put_str("pic_init done\n");
}

static void make_idt_desc(ST_GATE_DESC *p_gdesc, U8 attr,
                          intr_handler function) {
  p_gdesc->func_offset_low_word = (U32)function & 0x000000000000FFFF;
  p_gdesc->func_offset_high_word = ((U32)function & 0x00000000FFFF0000) >> 16;
  p_gdesc->attribute = attr;
  p_gdesc->dcount = 0;
  p_gdesc->selector = SELECTOR_K_CODE;
}

static void idt_desc_init(void) {
  int i;
  for (i = 0; i < IDT_DESC_CNT; i++) {
    make_idt_desc(&idt[i], IDT_DESC_ATTR_DPL0, intr_entry_table[i]);
  }
  put_str("idt_desc_init donE\n");
}

static void general_intr_handler(U8 vec_nr) {
  if (0x27 == vec_nr || 0x2f == vec_nr) {
    return;
  }
  put_str("int vector: 0x");
  put_int(vec_nr);
  put_char('\n');
}

static void exception_init(void) {
  int i;
  for (i = 0; i < IDT_DESC_CNT; i++) {
    idt_table[i] = general_intr_handler;
    intr_name[i] = "unknow";
  }
  intr_name[0] = "#DE Divide Error";
  intr_name[1] = "#DB Debug Exception";
  intr_name[2] = "NMI Interrupt";
  intr_name[3] = "#BP Breakpoint Exception";
  intr_name[4] = "#OF Overflow Exception";
  intr_name[5] = "#BR Bound Range Exceeded Exception";
  intr_name[6] = "#UD Invalid Opcode Exception";
  intr_name[7] = "#NM Device Not Available Exception";
  intr_name[8] = "#DF Double Fault Exception";
  intr_name[9] = "COPROCESSOR Segment Overrun";
  intr_name[10] = "#TS Invalid Tss Exception";
  intr_name[11] = "#NP Segment Not Present";
  intr_name[12] = "#SS Stack Fault Exception";
  intr_name[13] = "#GP General Protection Exception";
  intr_name[14] = "#PF Page-Fault Exception";
  // intr_name[15] = "";
  intr_name[16] = "#MF 0x87 Fpu Floating-Point Error";
  intr_name[17] = "#AC Alignment Check Exception";
  intr_name[18] = "#MC Machine-Check Exception";
  intr_name[19] = "#XF Simd Floating-Point Exception";
}

void idt_init() {
  put_str("idt_init start\n");
  idt_desc_init();
  exception_init();
  pic_init();

  U64 idt_operand = ((sizeof(idt) - 1) | ((U64)((U32)idt << 16)));
  asm volatile("lidt %0" ::"m"(idt_operand));
  put_str("idt_init done\n");
}

// enable interrupt
EN_INTR_STATUS intr_enable() {
  EN_INTR_STATUS old_status;
  if (EN_INTR_ON == intr_get_status()) {
    old_status = EN_INTR_ON;
  } else {
    old_status = EN_INTR_OFF;
    asm volatile("sti");
  }
  return old_status;
}

// disable interrupt
EN_INTR_STATUS intr_disable() {
  EN_INTR_STATUS old_status;
  if (EN_INTR_ON == intr_get_status()) {
    old_status = EN_INTR_ON;
    asm volatile("cli" ::: "memory");
  } else {
    old_status = EN_INTR_OFF;
  }
  return old_status;
}

// set interrupt status
EN_INTR_STATUS intr_set_status(EN_INTR_STATUS intr_status) {
  return intr_status & EN_INTR_ON ? intr_enable() : intr_disable();
}

// get interrupt status
EN_INTR_STATUS intr_get_status() {
  U32 eflags = 0;
  GET_FLAGS(eflags);
  return (EFLAGS_IF & eflags) ? EN_INTR_ON : EN_INTR_OFF;
}
