#include "main.h"

int add_quote(t_db *db, t_quote **head, int ascii, int start)
{
    t_quote *new;
    t_quote *tmp;

    new = gc_malloc(db, sizeof(t_quote));
    CATCH_ONNULL(new, error(db, NULL, "Malloc failed"));
    new->ascii = ascii;
    new->start = start;
    new->end = -1;
    new->next = NULL;
    if (!*head)
    {
        *head = new;
        return (SUCCESS);
    }
    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = new;
    return (SUCCESS);
}

t_quote *last_quote(t_quote *head)
{
    t_quote *tmp;

    if (!head) return (NULL);
    tmp = head;
    while (tmp->next)
        tmp = tmp->next;
    return (tmp);
}

int track_quotes(t_db *db, t_quote **head, char *line)
{
    int i;
    t_quote *last;

    i = 0;
    while (line[i])
    {
        last = last_quote(*head);
        if ((line[i] == SNGLQUOTE || line[i] == DOBLQUOTE) && !last)
        {
            CATCH_ONFAILURE(add_quote(db, head, line[i], i), FAILURE);
        }
        else if (
            (line[i] == SNGLQUOTE || line[i] == DOBLQUOTE)
            && last->ascii == line[i]
            && last->end == -1)
            {
                last->end = i;
            }
        else if ((line[i] == SNGLQUOTE || line[i] == DOBLQUOTE)
            && last->end != -1)
        {
            CATCH_ONFAILURE(add_quote(db, head, line[i], i), FAILURE);
        }
        i++;
    }
    last = last_quote(*head);
    if (last && last->end == -1)
    {
        printf("Quotes are not closed expecting %c\n", last->ascii);
        return error(db, NULL, NULL);
    }

    return (SUCCESS);
}

void reset_quotes(t_db *db, t_quote **head)
{
    t_quote *curr;
    t_quote *tmp;

    curr = *head;
    while (curr)
    {
        tmp = curr;
        curr = (curr)->next;
        gc_free(db, tmp);
    }
    *head = NULL;
}
