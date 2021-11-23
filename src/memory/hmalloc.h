#pragma once


#include "free_list.h"
#include <stdlib.h>

//Chained list of all the addresses "ptr"

struct free_list
{
    void * ptr;
    struct free_list *next;
};

extern struct free_list *list_malloc;

//Malloc a ptr of size "size" and add it in list_malloc

void *hmalloc(size_t size);

//Realloc "ptr" with size "size" and change his address in list_malloc 

void *hrealloc(void *ptr, size_t size);

//Malloc a ptr of size "size" and add it in list_malloc

void *hcalloc(size_t nmb, size_t size);

//Free the address "ptr" and remove it from list_malloc

void hfree(void *ptr);

//Free all the forgotten address in list_malloc

void free_all(void);
