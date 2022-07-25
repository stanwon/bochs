#ifndef __TSS_H_
#define __TSS_H_
#include "thread.h"

void tss_init();
void update_tss_esp(ST_TASK_STRUCT *pthread);

#endif
