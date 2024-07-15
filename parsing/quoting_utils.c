#include "main.h"
#include "parsing.h"

int inside_single_quote(t_quote *head, int   i)
{
    t_quote *q;

    q = head;
    while (q)
    {
        if (q->ascii == SNGLQUOTE && i >= q->start && i <= q->end)
            return (TRUE);
        q = q->next;
    }
    return (FALSE);
}

int is_inside_quotes(t_quote *head, int i)
{
    t_quote *tmp;

    tmp = head;
    while (tmp)
    {
        if (i >= tmp->start && i <= tmp->end)
            return (TRUE);
        tmp = tmp->next;
    }
    return (FALSE);
}
