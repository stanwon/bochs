#ifndef LIB_KERNEL_MEMORY_H
#define LIB_KERNEL_MEMORY_H
#include "bitmap.h"
#include "stdint.h"

typedef struct tag_virtual_addr {
  ST_BITMAP vaddr_bitmap;
  U32 vaddr_start;
} ST_VIRTUAL_ADDR;

typedef struct tag_pool {
  ST_BITMAP pool_bitmap;
  U32 phy_addr_start;
  U32 pool_size;
} ST_POOL;

extern ST_POOL kernel_pool, user_pool;
void mem_init(void);

#endif
