#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

typedef struct list List;
typedef void       *ListItem;

List    *list_init();
ListItem list_item(List *l);
List    *list_next(List *l);
List    *list_push_front(List *l, ListItem item);
void     list_destroy(List *l);

#endif