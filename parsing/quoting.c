#include "../includes/main.h"

int add_quote(t_db *db, int ascii, int start)
{
    t_quote *new;
    t_quote *tmp;

    if (!db->quotes)
    {
        new = gc_malloc(db, sizeof(t_quote));
        if (!new) return (FAILURE);
        new->ascii = ascii;
        new->start = start;
        new->end = -1;
        new->next = NULL;
        db->quotes = new;
        return (SUCCESS);
    }
    tmp = db->quotes;
    while (tmp->next)
        tmp = tmp->next;
    new = gc_malloc(db, sizeof(t_quote));
    if (!new) return (FAILURE);
    new->ascii = ascii;
    new->start = start;
    new->end = -1;
    new->next = NULL;
    return (SUCCESS);
}

t_quote *last_quote(t_db *db)
{
    t_quote *tmp;

    if (!db->quotes) return (NULL);
    tmp = db->quotes;
    while (tmp->next)
        tmp = tmp->next;
    return (tmp);
}

int track_quotes(t_db *db, char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if ((line[i] == 34 || line[i] == 39)
            && last_quote(db)->ascii == line[i]
            && last_quote(db)->end == -1)
            last_quote(db)->end = i;
        else if ((line[i] == 34 || line[i] == 39)
            && last_quote(db)->end != -1)
        {
            if (add_quote(db, line[i], i) == FAILURE) return (FAILURE);
        }
        i++;
    }
    if (last_quote(db)->end == -1) return (error(db, "Quotes are not closed"));
    return (SUCCESS);
}
