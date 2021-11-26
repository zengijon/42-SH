#include "../memory/hmalloc.h"
#include "stdio.h"

struct free_list *list_malloc = NULL;
int main()
{
    for (int i = 0; i < 4200; ++i)
        hmalloc(1500);

    void *tmp = hcalloc(1200, 5);
    hcalloc(1200, 5);
    void *tmp2 = hcalloc(1200, 5);
    void *tmp3 = hcalloc(1200, 5);

    tmp2 = hrealloc(tmp2, 500);
    tmp = hrealloc(tmp, 50000);
    tmp3 = hrealloc(tmp3, 600);
    
    hfree(tmp);
    hfree(tmp2);
    free_all();
    return 0;
}
