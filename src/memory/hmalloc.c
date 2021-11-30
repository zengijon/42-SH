#include "hmalloc.h"

#include "err.h"

void *hmalloc(size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        errx(1, "memory exausted");
    list_add(ptr);
    return ptr;
}

void *hcalloc(size_t nmb, size_t size)
{
    void *ptr = calloc(nmb, size);
    if (!ptr)
        errx(1, "memory exausted");
    list_add(ptr);
    return ptr;
}

void *hrealloc(void *ptr, size_t size)
{
    void *ptr2 = realloc(ptr, size);
    if (!ptr2)
        errx(1, "memory exausted");
    list_add(ptr2);
    if (ptr == NULL)
        list_remove(ptr);
    return ptr2;
}

void hfree(void *ptr)
{
    free(ptr);
    list_remove(ptr);
}

void free_all(void)
{
    while (list_malloc)
        free_head();
}
