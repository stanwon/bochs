#ifndef __CONSOLE_H
#define __CONSOLE_H
#include "stdint.h"

void console_init();
void console_put_str(char *str);
void console_put_char(U8 char_asci);
void console_put_int(U32 num);

#endif
