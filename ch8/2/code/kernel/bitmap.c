#include "../include/bitmap.h"
#include "../include/debug.h"
#include "../include/interrupt.h"
#include "../include/print.h"
#include "../include/stdint.h"
#include "../include/string.h"

void bitmap_init(ST_BITMAP *btmp) {
  memset(btmp->bits, 0, btmp->btmp_bytes_len);
}

bool bitmap_scan_test(ST_BITMAP *btmp, U32 bit_idx) {
  U32 byte_idx = bit_idx / 8;
  U32 bit_odd = bit_idx % 8;
  return (btmp->bits[byte_idx] & (BITMAP_MASK << bit_odd));
}

int bitmap_scan(ST_BITMAP *btmp, U32 cnt) {
  U32 idx_byte = 0;
  while ((idx_byte < btmp->btmp_bytes_len) && (0xff == btmp->bits[idx_byte])) {
    idx_byte++;
  }

  ASSERT(idx_byte < btmp->btmp_bytes_len);
  if (idx_byte == btmp->btmp_bytes_len) {
    return -1;
  }


  int idx_bit = 0;
  while ((U8)(BITMAP_MASK << idx_bit) & btmp->bits[idx_byte]) {
    idx_bit++;
  }

  int idx_bit_start = idx_byte * 8 + idx_bit;
  if (1 == cnt) {
    return idx_bit_start;
  }

  U32 bit_left = (btmp->btmp_bytes_len * 8 - idx_bit_start);
  U32 bit_next = idx_bit_start + 1;
  U32 count = 1;

  idx_bit_start = -1;
  while (bit_left-- > 0) {
    if (!(bitmap_scan_test(btmp, bit_next))) {
      count++;
    } else {
      count = 0;
    }

    if (count == cnt) {
      idx_bit_start = bit_next - cnt + 1;
      break;
    }
    bit_next++;
  }

  return idx_bit_start;
}

void bitmap_set(ST_BITMAP *btmp, U32 bit_idx, S8 value) {
  ASSERT((0 == value) || (1 == value));
  U32 byte_idx = bit_idx / 8;
  U32 bit_odd = bit_idx % 8;

  if (value) {
    btmp->bits[byte_idx] |= (BITMAP_MASK << bit_odd);
  } else {
    btmp->bits[byte_idx] &= ~(BITMAP_MASK << bit_odd);
  }
}
