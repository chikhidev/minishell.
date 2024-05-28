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

t_parnth *last_unclosed_paranth(t_db *db)
{
    t_parnth *tmp;
    t_parnth *last;

    last = NULL;
    tmp = db->paranthesis;
    while (tmp)
    {
        if (tmp->close_ == -1)
            last = tmp;
        tmp = tmp->next;
    }
    return (last);
}


int track_paranthesis(t_db *db, char *line)
{
    t_parnth    *last_opened;
    int         i;

    i = 0;
    while (line[i])
    {
        last_opened = last_unclosed_paranth(db);
        if (line[i] == '(' && !is_inside_quotes(db, i))
        {
            if (create_paranth(db, i) == FAILURE)
                return (FAILURE);
        }
        else if (line[i] == ')'
            && !is_inside_quotes(db, i)
            && last_opened
            && last_opened->open_ == i - 1)
            return error(db, "syntax error: empty paranthesis");
        else if (line[i] == ')'
            && !is_inside_quotes(db, i)
            && last_opened)
        {
            if (!db->paranthesis)
                return error(db, "syntax error: no paranthesis to close");
            last_opened->close_ = i;
        }
        else if (line[i] == ')' && !is_inside_quotes(db, i))
            return error(db, "syntax error: no paranthesis to close");
        i++;
    }
    if (last_unclosed_paranth(db))
        return error(db, "syntax error: some paranthesis are not closed");
    return (SUCCESS);
}
