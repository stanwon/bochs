#include "../include/memory.h"
#include "../include/bitmap.h"
#include "../include/debug.h"
#include "../include/global.h"
#include "../include/print.h"
#include "../include/string.h"

#define PG_SIZE 4096
#define MEM_BITMAP_BASE 0xc009a000
#define K_HEAP_START 0xc0100000
#define PDE_IDX(addr) ((addr & 0xffc00000) >> 22)
#define PTE_IDX(addr) ((addr & 0x003ff000) >> 12)

ST_POOL kernel_pool;
ST_POOL user_pool;
ST_VIRTUAL_ADDR kernel_vaddr;

static void *vaddr_get(EN_POOL_FLAG pf, U32 pg_cnt) {
  int vaddr_start = 0;
  int bit_idx_start = -1;
  U32 cnt = 0;
  if (pf == EN_PF_KERNEL) {
    bit_idx_start = bitmap_scan(&kernel_vaddr.vaddr_bitmap, pg_cnt);
    if (-1 == bit_idx_start) {
      return NULL;
    }

    while (cnt < pg_cnt) {
      bitmap_set(&kernel_pool.pool_bitmap, bit_idx_start + cnt++, 1);
    }
    vaddr_start = kernel_vaddr.vaddr_start + bit_idx_start * PG_SIZE;
  } else {
  }
  return (void *)vaddr_start;
}

U32 *pte_ptr(U32 vaddr) {
  U32 *pte =
      (U32 *)(0xffc00000 + ((vaddr & 0xffc00000) >> 10) + PTE_IDX(vaddr) * 4);
  return pte;
}

U32 *pde_ptr(U32 vaddr) {
  U32 *pde = (U32 *)(0xfffff000 + PDE_IDX(vaddr) * 4);
  return pde;
}

static void *palloc(ST_POOL *m_pool) {
  int bit_idx = bitmap_scan(&m_pool->pool_bitmap, 1);
  put_int(bit_idx);
  if (-1 == bit_idx) {
    return NULL;
  }
  bitmap_set(&m_pool->pool_bitmap, bit_idx, 1);
  U32 page_phyaddr = ((bit_idx * PG_SIZE) + m_pool->phy_addr_start);
  return (void *)page_phyaddr;
}

static void page_table_add(void *_vaddr, void *_page_phyaddr) {
  U32 vaddr = (U32)_vaddr;
  U32 page_phyaddr = (U32)_page_phyaddr;
  U32 *pde = pde_ptr(vaddr);
  U32 *pte = pte_ptr(vaddr);

  if (*pde & 0x00000001) {
    ASSERT(!(*pte & 0x00000001));
    if (!(*pte & 0x00000001)) {
      *pte = (page_phyaddr | PG_US_U | PG_RW_W | PG_P_1);
    } else {
      PANIC("pte repeat");
      *pte = (page_phyaddr | PG_US_U | PG_RW_W | PG_P_1);
    }
  } else {
    U32 pde_phyaddr = (U32)palloc(&kernel_pool);
    *pde = (pde_phyaddr | PG_US_U | PG_RW_W | PG_P_1);
    memset((void *)((int)pte & 0xfffff000), 0, PG_SIZE);
    ASSERT(!(*pte & 0x00000001));
    *pte = (page_phyaddr | PG_US_U | PG_RW_W | PG_P_1);
  }
}

void *malloc_page(EN_POOL_FLAG pf, U32 pg_cnt) {
  ASSERT(pg_cnt > 0 && pg_cnt < 3840);
  void *vaddr_start = vaddr_get(pf, pg_cnt);
  if (NULL == vaddr_start) {
    return NULL;
  }
  U32 vaddr = (U32)vaddr_start;
  U32 cnt = pg_cnt;
  ST_POOL *mem_pool = pf & EN_PF_KERNEL ? &kernel_pool : &user_pool;

  while (cnt-- > 0) {
    void *page_phyaddr = palloc(mem_pool);
    if (NULL == page_phyaddr) {
      return NULL;
    }
    page_table_add((void *)vaddr, page_phyaddr);
    vaddr += PG_SIZE;
  }
  return vaddr_start;
}

void *get_kernel_pages(U32 pg_cnt) {
  void *vaddr = malloc_page(EN_PF_KERNEL, pg_cnt);
  if (NULL != vaddr) {
    memset(vaddr, 0, pg_cnt * PG_SIZE);
  }
  return vaddr;
}

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

  kernel_pool.pool_size = kernel_free_pages * PG_SIZE;
  user_pool.pool_size = user_free_pages * PG_SIZE;

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

void mem_init() {
  put_str("mem_init start\n");
  U32 mem_bytes_total = (*(U32 *)(0xb00));
  mem_pool_init(mem_bytes_total);
  put_str("mem_init done\n");
}
