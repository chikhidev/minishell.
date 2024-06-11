#include "../includes/main.h"
#include "../includes/parsing.h"

int is_inside_paranthesis(t_parnth *head, int i)
{
    for (t_parnth *p = head; p; p = p->next)
    {
        if (i > p->open_ && i < p->close_)
            return 1;
    }
    return 0;
}
