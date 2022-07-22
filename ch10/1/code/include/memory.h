#ifndef LIB_KERNEL_MEMORY_H
#define LIB_KERNEL_MEMORY_H
#include "bitmap.h"
#include "stdint.h"

typedef enum pool_flag{
  EN_PF_KERNEL = 1,
  EN_PF_USER = 2
} EN_POOL_FLAG;

#define PG_P_1 1
#define PG_P_0 0
#define PG_RW_R 0
#define PG_RW_W 2
#define PG_US_S 0
#define PG_US_U 4

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
void *get_kernel_pages(U32 pg_cnt);

#endif
