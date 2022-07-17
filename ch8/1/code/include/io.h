#ifndef __LIB_IO_H
#define __LIB_IO_H
#include "stdint.h"

static inline void outb(U16 port, U8 data) {
  asm volatile("outb %b0, %w1" ::"a"(data), "Nd"(port));
}

static inline void outsw(U16 port, const void *addr, U32 word_cnt) {
  asm volatile("cld; rep outsw" : "+S"(addr), "+c"(word_cnt) : "d"(port));
}

static inline U8 inb(U16 port) {
  U8 data;
  asm volatile("inb %w1, %b0" : "+a"(data) : "Nd"(port));
  return data;
}

static inline void insw(U16 port, const void *addr, U32 word_cnt) {
  asm volatile("cld; rep insw"
               : "+D"(addr), "+c"(word_cnt)
               : "d"(port)
               : "memory");
}

#endif
