#include "../include/memory.h"
#include "../include/print.h"
#include "../include/stdint.h"

#define PG_SIZE 4096
#define MEM_BITMAP_BASE 0xc009a000
#define K_HEAP_START 0xc0100000

typedef struct tag_pool {
  ST_BITMAP poo_bitmap;
  U32 phy_addr_start;
  U32 pool_size;
} ST_POOL;

ST_POOL kernel_pool;
ST_POOL user_pool;
ST_VIRTUAL_ADDR kernel_vaddr;

static void mem_pool_init(U32 all_mem) {
  put_str("mem_pool_init start\n");
  U32 page_table_size = PG_SIZE * 256;
  U32 used_mem = page_table_size + 0x100000;
  U32 free_mem = all_mem - used_mem;
  U16 all_free_pages = free_mem / PG_SIZE;
}
