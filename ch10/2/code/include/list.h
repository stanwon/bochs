#ifndef __LIST_H
#define __LIST_H
#include "global.h"

#define offset(struct_type, member) (int)(&((struct_type *)0)->member)
#define elem2entry(struct_type, struct_member_name, elem_ptr) \
  (struct_type *)((int)elem_ptr - offset(struct_type, struct_member_name))

typedef struct tag_list_elem{
  struct tag_list_elem *prev;
  struct tag_list_elem *next;
} ST_LIST_ELEM;

typedef struct tag_list{
  ST_LIST_ELEM head;
  ST_LIST_ELEM tail;
} ST_LIST;

typedef bool (function)(ST_LIST_ELEM *, int arg);;

void list_init(ST_LIST *list);
void list_insert_before(ST_LIST_ELEM *before, ST_LIST_ELEM *elem);
void list_push(ST_LIST *plist, ST_LIST_ELEM *elem);
void list_append(ST_LIST *plist, ST_LIST_ELEM *elem);
void list_remove(ST_LIST_ELEM *pelem);
ST_LIST_ELEM *list_pop(ST_LIST *plist);
bool elem_find(ST_LIST *plist, ST_LIST_ELEM *obj_elem);
ST_LIST_ELEM *list_traversal(ST_LIST *plist, function func, int arg);
U32 list_len(ST_LIST *plist);
bool list_empty(ST_LIST *plist);

#endif
