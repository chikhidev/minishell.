#include "../includes/main.h"
#include "../includes/parsing.h"

int inside_single_quote(t_quote *head, int   i)
{
    t_quote *q;

    q = head;
    while (q)
    {
        if (q->ascii == SNGLQUOTE && i >= q->start && i <= q->end)
            return (1);
        q = q->next;
    }
    return (0);
}

int is_inside_quotes(t_quote *head, int i)
{
    t_quote *tmp;

    tmp = head;
    while (tmp)
    {
        if (i >= tmp->start && i <= tmp->end)
            return (1);
        tmp = tmp->next;
    }
    return (0);
}
