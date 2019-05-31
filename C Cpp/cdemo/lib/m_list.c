#include "m_list.h"

#define sta_log printf

static list_free_function list_free_func = NULL;

void list_init(list_T* list,list_free_function free_func)
{
    list->data = NULL;
    list->next = NULL;
    list_free_func = free_func;
}

bool list_add(list_T* list,void *data)
{
    if(list == NULL) {
        sta_log("List is NULL.");
        return FALSE;
    }

    if(data == NULL) {
        sta_log("data is NULL.");
        return FALSE;
    }

    // New list item and add data into list item.
    list_T* list_new = (list_T*)malloc(sizeof(list_T));
    if(list_new == NULL) {
        sta_log("malloc list_T fail.");
        return NULL;
    }
    list_new->next = NULL;
    list_new->data = data;

    // Jump to list end.
    list_T* list_p = list;
    while(list_p->next){
        list_p = list_p->next;
    }

    // End item point to new list item.
    list_p->next = list_new;

    return TRUE;
}

#if 0
#define list_del(list,data_type,data_item_type,data_item_name,data_item_value) \
bool list_del_fun(list_T *list,data_item_type data_item_value) \
{ \
    if(list == NULL) { \
        sta_log("List is NULL."); \
        return FALSE; \
    } \
    list_T *list_p = list->next; \
    while(list_p) \
    { \
        data_type *data = (data_type*)(list_p->data); \
        if(data->data_item_name == data_item_value) \
        { \

            return TRUE; \
        } \
        list_p = list_p->next; \
    } \
    return FALSE; \
}
#endif

void list_free(list_T *list)
{
    if(list == NULL) {
        sta_log("List is NULL.");
        return;
    }

    list_T* list_p = list->next;
    while(list_p){
        if(list_free_func != NULL)
            list_free_func(list_p->data);

        list_T* list_temp = list_p->next;
        free(list_p);
        sta_log("Free list item : %p \n",list_p);
        list_p = list_temp;
    }
}
