#ifndef __PROCESS_H_
#define __PROCESS_H_

void process_activate(ST_TASK_STRUCT *p_thread);
void process_execute(void *filename, char *name);

#endif
