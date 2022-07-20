#ifndef LIB_KERNEL_STRING_H
#define LIB_KERNEL_STRING_H

#include "stdint.h"

void memset(void *dst_, U8 value, U32 size);
void memcpy(void *dst_, const void *src_, U32 size);
int memcmp(const void *a_, const void *b_, U32 size);
char *strcpy(char *dst_, const char *src_);
U32 strlen(const char *str);
S8 strcmp(const char *a, const char *b);
char *strchr(const char *str, const U8 ch);
char *strrchr(const char *str, const U8 ch);
char *strcat(char *dst_, const char *src_);
U32 strchrs(const char *str, U8 ch);

#endif
