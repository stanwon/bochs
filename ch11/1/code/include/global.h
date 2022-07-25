#ifndef LIB_GLOBAL_H
#define LIB_GLOBAL_H
#include "stdint.h"

#define RPL0 0
#define RPL1 1
#define RPL2 2
#define RPL3 3

#define DESC_G_4K 1
#define DESC_D_32 1
#define DESC_L 0
#define DESC_AVL 0
#define DESC_P 1
#define DESC_DPL_0 0
#define DESC_DPL_1 1
#define DESC_DPL_2 2
#define DESC_DPL_3 3

#define DESC_S_CODE 1
#define DESC_S_DATA DESC_S_CODE
#define DESC_S_SYS 0
#define DESC_TYPE_CODE 8
#define DESC_TYPE_DATA 2
#define DESC_TYPE_TSS 9

#define TI_GDT 0
#define TI_LGT 1

#define SELECTOR_K_CODE ((1 << 3) + (TI_GDT << 2) + RPL0)
#define SELECTOR_K_DATA ((2 << 3) + (TI_GDT << 2) + RPL0)
#define SELECTOR_K_STACK SELECTOR_K_DATA
#define SELECTOR_K_GS ((3 << 3) + (TI_GDT << 2) + RPL0)

#define SELECTOR_U_CODE ((5 << 3) + (TI_GDT << 2) + RPL3)
#define SELECTOR_U_DATA ((6 << 3) + (TI_GDT << 2) + RPL3)
#define SELECTOR_U_STACK SELECTOR_U_DATA

#define GDT_ATTR_HIGH                                                          \
  ((DESC_G_4K << 7) + (DESC_D_32 << 6) + (DESC_L << 5) + (DESC_AVL << 4))

#define GDT_CODE_ATTR_LOW_DPL3                                                 \
  ((DESC_P << 7) + (DESC_DPL_3 << 5) + (DESC_S_CODE << 4) + DESC_TYPE_CODE)

#define GDT_DATA_ATTR_LOW_DPL3                                                 \
  ((DESC_P << 7) + (DESC_DPL_3 << 5) + (DESC_S_DATA << 4) + DESC_TYPE_DATA)

#define TSS_DESC_D 0
#define TSS_ATTR_HIGH                                                          \
  ((DESC_G_4K << 7) + (TSS_DESC_D << 6) + (DESC_L << 5) + (DESC_AVL << 4) + 0x0)

#define TSS_ATTR_LOW                                                           \
  ((DESC_P << 7) + (DESC_DPL_0 << 5) + (DESC_S_SYS << 4) + DESC_TYPE_TSS)

#define SELECTOR_TSS ((4 << 3) + (TI_GDT << 2) + RPL0)

typedef struct tag_gdt_desc {
  U16 limit_low_word;
  U16 base_low_word;
  U8 base_mid_byte;
  U8 attr_low_byte;
  U8 limit_high_attr_high;
  U8 base_high_byte;
} ST_GDT_DESC;

#define IDT_DESC_P 1
#define IDT_DESC_DPL0 0
#define IDT_DESC_DPL3 3
#define IDT_DESC_32_TYPE 0xE
#define IDT_DESC_16_TYPE 0x6

#define IDT_DESC_ATTR_DPL0                                                     \
  ((IDT_DESC_P << 7) + (IDT_DESC_DPL0 << 5) + IDT_DESC_32_TYPE)
#define IDT_DESC_ATTR_DPL3                                                     \
  ((IDT_DESC_P << 7) + (IDT_DESC_DPL3 << 5) + IDT_DESC_32_TYPE)

#define out_int(a)                                                             \
  put_str("[");                                                                \
  put_str(__FILE__);                                                           \
  put_str("] ");                                                               \
  put_str(__FUNCTION__);                                                       \
  put_str("() <0x");                                                           \
  put_int(__LINE__);                                                           \
  put_str(">: ");                                                              \
  put_str(#a);                                                                 \
  put_str(" = ");                                                              \
  put_int(a);                                                                  \
  put_str("\n");

#define EFLAGS_MBS (1 << 1)
#define EFLAGS_IF_1 (1 << 9)
#define EFLAGS_IF_0 0
#define EFLAGS_IOPL_3 (3 << 12)
#define EFLAGS_IOPL_0 (0 << 12)

#define DIV_ROUND_UP(X, STEP) ((X + STEP - 1) / (STEP))

#endif
