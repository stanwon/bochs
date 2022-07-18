#include "../../include/string.h"
#include "../../include/debug.h"
#include "../../include/global.h"

void memset(void *dst_, U8 value, U32 size) {
  ASSERT(NULL != dst_);
  U8 *dst = (U8 *)dst_;
  while (size-- > 0) {
    *dst++ = value;
  }
}

void memcpy(void *dst_, const void *src_, U32 size) {
  ASSERT(NULL != dst_ && NULL != src_);
  U8 *dst = dst_;
  const U8 *src = src_;
  while (size-- > 0) {
    *dst++ = *src++;
  }
}

int memcmp(const void *a_, const void *b_, U32 size) {
  ASSERT(NULL != a_ && NULL != b_);
  const char *a = a_;
  const char *b = b_;
  while (size-- > 0) {
    if (*a != *b) {
      return *a > *b ? 1 : -1;
    }
    a++;
    b++;
  }
  return 0;
}

char *strcpy(char *dst_, const char *src_) {
  ASSERT(NULL != dst_ && NULL != src_);
  char *r = dst_;
  while ((*dst_++ = *src_++))
    ;
  return r;
}

U32 strlen(const char *str) {
  ASSERT(NULL != str);
  const char *p = str;
  while (*p++)
    ;
  return (p - str - 1);
}

S8 strcmp(const char *a, const char *b) {
  ASSERT(NULL != a && NULL != b);
  while (*a != 0 && *a == *b) {
    a++;
    b++;
  }
  return *a < *b ? -1 : *a > *b;
}

char *strchr(const char *str, const U8 ch) {
  ASSERT(NULL != str);
  while (*str != 0) {
    if (*str == ch) {
      return (char *)str;
    }
    str++;
  }
  return NULL;
}

char *strrchr(const char *str, const U8 ch) {
  ASSERT(NULL != str);
  const char *last_char = NULL;
  while (*str != 0) {
    if (*str == ch) {
      last_char = str;
    }
    str++;
  }
  return (char *)last_char;
}

char *strcat(char *dst_, const char *src_) {
  ASSERT(NULL != dst_ && NULL != src_);
  char *str = dst_;
  while (*str++)
    ;
  --str;
  while ((*str++ = *src_++))
    ;
  return dst_;
}

U32 strchrs(const char *str, U8 ch) {
  ASSERT(NULL != str);
  U32 ch_cnt;
  const char *p = str;
  while (*p != 0) {
    if (*p == ch) {
      ch_cnt++;
    }
    p++;
  }
  return ch_cnt;
}
