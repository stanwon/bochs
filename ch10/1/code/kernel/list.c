#include "../include/list.h"
#include "../include/interrupt.h"
#include "../include/stdint.h"
#include "../include/print.h"

void list_init(ST_LIST *list) {
  list->head.prev = NULL;
  list->head.next = &list->tail;
  list->tail.prev = &list->head;
  list->tail.next = NULL;
}

void list_insert_before(ST_LIST_ELEM *before, ST_LIST_ELEM *elem) {
  EN_INTR_STATUS old_status = intr_disable();

  before->prev->next = elem;
  elem->prev = before->prev;
  elem->next = before;

  before->prev = elem;

  intr_set_status(old_status);
}

void list_push(ST_LIST *plist, ST_LIST_ELEM *elem) {
  list_insert_before(plist->head.next, elem);
}

void list_append(ST_LIST *plist, ST_LIST_ELEM *elem) {
  list_insert_before(&plist->tail, elem);
}

void list_remove(ST_LIST_ELEM *pelem) {
  EN_INTR_STATUS old_status = intr_disable();
  pelem->prev->next = pelem->next;
  pelem->next->prev = pelem->prev;
  intr_set_status(old_status);
}

ST_LIST_ELEM *list_pop(ST_LIST *plist) {
  ST_LIST_ELEM *elem = plist->head.next;
  list_remove(elem);
  return elem;
}

bool elem_find(ST_LIST *plist, ST_LIST_ELEM *obj_elem) {
  ST_LIST_ELEM *elem = plist->head.next;
  while (elem != &plist->tail) {
    if (elem == obj_elem) {
      return true;
    }
    elem = elem->next;
  }
  return false;
}

ST_LIST_ELEM *list_traversal(ST_LIST *plist, function func, int arg) {
  ST_LIST_ELEM *elem = plist->head.next;
  if (list_empty(plist)) {
    return NULL;
  }
  while (elem != &plist->tail) {
    if (func(elem, arg)) {
      return elem;
    }
    elem = elem->next;
  }
  return NULL;
}

U32 list_len(ST_LIST *plist) {
  ST_LIST_ELEM *elem = plist->head.next;
  U32 length = 0;

  while (elem != &plist->tail) {
    length++;
    elem = elem->next;
  }
  return length;
}

bool list_empty(ST_LIST *plist) {
  return (plist->head.next == &plist->tail) ? true : false;
}
