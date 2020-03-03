#ifndef M_LIST_H
#define M_LIST_H
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "m_type.h"

#include "../config"

typedef void (*list_free_function)(void *data);

typedef struct list_T {
    struct list_T *next;

    void *data;
} list_T;

#define list_for_each(list,data_type,data_name)         \
    list_T *list_p = NULL; \
    data_type* data_name = NULL; \
    for (list_p = (list)->next,data_name = (list_p == NULL ? NULL : (data_type*)(list_p->data)); \
         list_p != NULL && data_name != NULL; \
         list_p = list_p->next,data_name = (list_p == NULL ? NULL : (data_type*)(list_p->data)))


extern void list_init(list_T *list,list_free_function free_func);
extern bool list_add(list_T *list,void *data);
extern void list_free(list_T *list);

#endif /* M_LIST_H */
