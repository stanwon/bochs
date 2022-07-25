#include "../include/global.h"
#include "../include/thread.h"
#include "../include/print.h"
#include <string.h>

typedef struct tag_tss {
  U32 backlink;
  U32 *esp0;
  U32 ss0;
  U32 *esp1;
  U32 ss1;
  U32 *esp2;
  U32 ss2;
  U32 cr3;
  U32 (*eip)(void);
  U32 eflags;
  U32 eax;
  U32 ecx;
  U32 edx;
  U32 ebx;
  U32 esp;
  U32 ebp;
  U32 esi;
  U32 edi;
  U32 es;
  U32 cs;
  U32 ss;
  U32 ds;
  U32 fs;
  U32 gs;
  U32 ldt;
  U32 trace;
  U32 io_base;
} ST_TSS;

static ST_TSS tss;

void update_tss_esp(ST_TASK_STRUCT *pthread) {
  tss.esp0 = (U32 *)((U32)pthread + PG_SIZE);
}

static ST_GDT_DESC make_gdt_desc(U32 *desc_addr, U32 limit, U8 attr_low,
                                 U8 attr_high) {
  U32 desc_base = (U32)desc_addr;
  ST_GDT_DESC desc;
  desc.limit_low_word = limit & 0x0000ffff;
  desc.base_low_word = desc_base & 0x0000ffff;
  desc.base_mid_byte = ((desc_base & 0x00ff0000) >> 16);
  desc.attr_low_byte = (U8)(attr_low);
  desc.limit_high_attr_high = (((limit & 0x000f0000) >> 16) + (U8)(attr_high));
  desc.base_high_byte = desc_base >> 24;
  return desc;
}

void tss_init() {
  put_str("tss_init start\n");
  U32 tss_size = sizeof(tss);
  memset(&tss, 0, tss_size);
  tss.ss0 = SELECTOR_K_STACK;
  tss.io_base = tss_size;
  *((ST_GDT_DESC *)0xc0000920) = make_gdt_desc((U32 *)&tss, tss_size -1, TSS_ATTR_LOW, TSS_ATTR_HIGH);
  *((ST_GDT_DESC *)0xc0000928) = make_gdt_desc((U32 *)0, 0xfffff, GDT_CODE_ATTR_LOW_DPL3, GDT_ATTR_HIGH);
  *((ST_GDT_DESC *)0xc0000930) = make_gdt_desc((U32 *)0, 0xfffff, GDT_DATA_ATTR_LOW_DPL3, GDT_ATTR_HIGH);

  U64 gdt_operand = ((8*7 -1)| ((U64)(U32)0xc0000900 << 16));
  asm volatile("lgdt %0"::"m"(gdt_operand));
  asm volatile("ltr %w0"::"r"(SELECTOR_TSS));

  put_str("tss_init and ltr done\n");
}
