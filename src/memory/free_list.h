#pragma once

#include <stdlib.h>

//Add the address "ptr" at the head of list_malloc

void list_add(void *ptr);

//Remove the address "ptr" of the list_malloc and free it

void list_remove(void *ptr);

//Free the first element of list_malloc

void free_head(void);
