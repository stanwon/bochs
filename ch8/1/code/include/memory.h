#ifndef LIB_KERNEL_MEMORY_H
#define LIB_KERNEL_MEMORY_H
#include "bitmap.h"
#include "stdint.h"

typedef struct tag_virtual_addr {
  ST_BITMAP vaddr_bitmap;
  U32 vaddr_start;
} ST_VIRTUAL_ADDR;

#endif
