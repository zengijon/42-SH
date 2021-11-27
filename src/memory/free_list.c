#include "free_list.h"

#include <err.h>

#include "hmalloc.h"

void list_add(void *ptr)
{
    struct free_list *res = calloc(sizeof(struct free_list), 1);
    if (!res)
        errx(1, "Can't calloc");
    res->ptr = ptr;
    res->next = list_malloc;
    list_malloc = res;
}

void list_remove(void *ptr)
{
    if (!list_malloc)
        return;
    struct free_list *tmp = list_malloc;
    if (tmp->ptr == ptr)
    {
        list_malloc = list_malloc->next;
        free(tmp);
        return;
    }
    while (tmp->next && tmp->next->ptr != ptr)
    {
        tmp = tmp->next;
    }
    if (tmp->next)
    {
        struct free_list *t = tmp->next;
        tmp->next = tmp->next->next;
        free(t);
    }
}

void free_head(void)
{
    struct free_list *tmp = list_malloc;
    list_malloc = list_malloc->next;
    free(tmp->ptr);
    free(tmp);
}
