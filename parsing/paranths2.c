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

int paranths_scopes_count(t_db *db)
{
    t_parnth *tmp;
    int count;

    count = 0;
    tmp = db->paranthesis;
    while (tmp)
    {
        if (tmp->close_ != -1)
            count++;
        tmp = tmp->next;
    }
    return (count);
}

int all_scopes_has_op_between(t_db *db, char *line)
{
    t_parnth *tmp;
    int     save;
    int     i;

    if (!db->paranthesis)
        return (TRUE);
    tmp = db->paranthesis;
    while (tmp)
    {
        i = tmp->close_ + 1;
        skip_spaces(line, &i);
        save = i;
        if (!line[i])
            return (TRUE);
        else if (line[i] == '('|| !is_valid_op(line[i], line[i + 1], &i))
        {
            printf("syntax error near unexpected token `%c'\n at %d\n", line[save], save);
            // printf("syntax error near unexpected token `%c'\n", line[save]);
            return (FALSE);
        }
        tmp = tmp->next;
    }
    return (TRUE);
}
