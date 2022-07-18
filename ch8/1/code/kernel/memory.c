#include "../include/memory.h"
#include "../include/print.h"
#include "../include/stdint.h"

#define PG_SIZE 4096
#define MEM_BITMAP_BASE 0xc009a000
#define K_HEAP_START 0xc0100000

ST_POOL kernel_pool;
ST_POOL user_pool;
ST_VIRTUAL_ADDR kernel_vaddr;

static void mem_pool_init(U32 all_mem) {
  put_str("mem_pool_init start\n");
  U32 page_table_size = PG_SIZE * 256;
  U32 used_mem = page_table_size + 0x100000;
  U32 free_mem = all_mem - used_mem;
  U16 all_free_pages = free_mem / PG_SIZE;
  U16 kernel_free_pages = all_free_pages / 2;
  U16 user_free_pages = all_free_pages - kernel_free_pages;

  U32 kbm_length = kernel_free_pages / 8;
  U32 ubm_length = user_free_pages / 8;

  U32 kp_start = used_mem;
  U32 up_start = kp_start + kernel_free_pages * PG_SIZE;

  kernel_pool.phy_addr_start = kp_start;
  user_pool.phy_addr_start = up_start;
  kernel_pool.pool_bitmap.btmp_bytes_len = kbm_length;
  user_pool.pool_bitmap.btmp_bytes_len = ubm_length;

  kernel_pool.pool_bitmap.bits = (void *)MEM_BITMAP_BASE;
  user_pool.pool_bitmap.bits = (void *)(MEM_BITMAP_BASE + kbm_length);
  put_str("kernel_pool_bitmap_start: ");
  put_int((int)kernel_pool.pool_bitmap.bits);
  put_str(" kernel_pool_phy_addr_start: ");
  put_int(kernel_pool.phy_addr_start);
  put_str("\n");
  put_str("user_pool_bitmap_start: ");
  put_int(user_pool.phy_addr_start);
  put_str("\n");

  bitmap_init(&kernel_pool.pool_bitmap);
  bitmap_init(&user_pool.pool_bitmap);

  kernel_vaddr.vaddr_bitmap.btmp_bytes_len = kbm_length;

  kernel_vaddr.vaddr_bitmap.bits =
      (void *)(MEM_BITMAP_BASE + kbm_length + ubm_length);

  kernel_vaddr.vaddr_start = K_HEAP_START;
  bitmap_init(&kernel_vaddr.vaddr_bitmap);
  put_str("mem_pool_init done\n");
}

void mem_init()
{
  put_str("mem_init start\n");
  U32 mem_bytes_total = (*(U32*)(0xb00));
  mem_pool_init(mem_bytes_total);
  put_str("mem_init done\n");
}
