#ifndef LIB_INTERRUPT_H
#define LIB_INTERRUPT_H
#include "stdint.h"

typedef void* intr_handler;
void idt_init(void);

typedef enum intr_status{
  EN_INTR_OFF,
  EN_INTR_ON,
} EN_INTR_STATUS;

EN_INTR_STATUS intr_get_status(void);
EN_INTR_STATUS intr_set_status(EN_INTR_STATUS intr_status);
EN_INTR_STATUS intr_enable(void);
EN_INTR_STATUS intr_disable(void);
void register_handler(U8 vector_no, intr_handler function);

#endif
