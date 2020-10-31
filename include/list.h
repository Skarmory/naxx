#ifndef YUN_LIST_H
#define YUN_LIST_H

#include <stdbool.h>

typedef struct _ListNode
{
    void* data;
    struct _ListNode* next;
    struct _ListNode* prev;
} ListNode;

typedef struct _List
{
    int count;
    ListNode* head;
    ListNode* tail;
} List;

List* list_new(void);
void list_free(List* list);
void list_init(List* list);
void list_uninit(List* list);
void list_add(List* list, void* data);
void list_rm(List* list, ListNode* node);
ListNode* list_find(List* list, void* data);
void list_splice_node(List* list_from, List* list_to, ListNode* node);
void* list_pop_head(List* list);
void* list_peek_head(List* list);
bool list_empty(List* list);

#define list_for_each(list, it)\
    for(it = (list)->head; it != NULL; it = it->next)

#define list_for_each_safe(list, it, n)\
    for(it = (list)->head, n = (it ? it->next : NULL); it != NULL; it = n, n = (it ? it->next : NULL))

#endif
