#include "list.h"

struct list {
    struct list *next;
    ListItem     item;
};

List *list_node(List *next, ListItem item) {
    List *l = malloc(sizeof(List));
    l->item = item;
    l->next = next;
    return l;
};

List *list_init() {
    return NULL;
}

ListItem list_item(List *l) {
    return l->item;
}

List *list_next(List *l) {
    return l->next;
}

List *list_push_front(List *l, ListItem item) {
    List *node = list_node(l, item);
    return node;
}

void list_destroy(List *l, ListItemDestroy destroy_fn) {
    while (l != NULL) {
        List *next = l->next;
        destroy_fn(l->item);
        free(l);
        l = next;
    }
}