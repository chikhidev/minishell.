#include "../includes/main.h"

int is_inside_quotes(t_db *db, int i)
{
    t_quote *tmp;

    tmp = db->quotes;
    while (tmp)
    {
        if (i >= tmp->start && i <= tmp->end)
            return (1);
        tmp = tmp->next;
    }
    return (0);
}

int create_paranth(t_db *db, int open_)
{
    t_parnth *new;
    t_parnth *tmp;

    new = gc_malloc(db, sizeof(t_parnth));
    if (!new) return (FAILURE);
    new->open_ = open_;
    new->close_ = -1;
    new->next = NULL;
    if (!db->paranthesis)
    {
        db->paranthesis = new;
        return (SUCCESS);
    }
    tmp = db->paranthesis;
    while (tmp->next)
        tmp = tmp->next;

    tmp->next = new;
    return (SUCCESS);
}

t_parnth *last_paranth(t_db *db)
{
    t_parnth *tmp;

    tmp = db->paranthesis;
    while (tmp->next)
        tmp = tmp->next;
    return (tmp);
}

int some_paranth_open(t_db *db)
{
    t_parnth *tmp;

    tmp = db->paranthesis;
    while (tmp)
    {
        if (tmp->close_ == -1)
            return (1);
        tmp = tmp->next;
    }
    return (0);
}

int track_paranthesis(t_db *db, char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if (line[i] == '(' && !is_inside_quotes(db, i))
        {
            if (create_paranth(db, i) == FAILURE)
                return (FAILURE);
        }
        else if (line[i] == ')'
            && !is_inside_quotes(db, i)
            && some_paranth_open(db))
        {
            if (!db->paranthesis)
                return error(db, "syntax error: no paranthesis to close");
            last_paranth(db)->close_ = i;
        }
        else if (line[i] == ')' && !is_inside_quotes(db, i))
            return error(db, "syntax error: no paranthesis to close");
        i++;
    }
    if (some_paranth_open(db))
        return error(db, "syntax error: some paranthesis are not closed");
    return (SUCCESS);
}