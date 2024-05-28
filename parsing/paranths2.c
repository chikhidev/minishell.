#include "../includes/main.h"
#include "../includes/parsing.h"

int is_inside_paranthesis(t_db *db, int i)
{
    t_parnth *tmp;

    tmp = db->paranthesis;
    while (tmp)
    {
        if (i >= tmp->open_ && i <= tmp->close_)
            return (1);
        tmp = tmp->next;
    }
    return (0);
}
