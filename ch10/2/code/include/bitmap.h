#ifndef LIB_KERNEL_BITMAP_H
#define LIB_KERNEL_BITMAP_H
#include "global.h"

#define BITMAP_MASK 1
typedef struct tag_bitmap {
  U32 btmp_bytes_len;
  U8 *bits;
} ST_BITMAP;

void bitmap_init(ST_BITMAP *btmp);
bool bitmap_scan_test(ST_BITMAP *btmp, U32 bit_idx);
int bitmap_scan(ST_BITMAP *btmp, U32 cnt);
void bitmap_set(ST_BITMAP *btmp, U32 bit_idx, S8 value);

#endif
