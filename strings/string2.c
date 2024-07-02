#include "../includes/main.h"
#include "../includes/parsing.h"

char *remove_paranthesis(t_db *db, char *line, t_parnth *local_paranths)
{
    char *new;
    int i;
    int j;

    new = gc_malloc(db, ft_strlen(line) - 1);
    i = 0;
    j = 0;
    while (line[i])
    {
        if (!(i == local_paranths->open_ || i == local_paranths->close_))
        {
            new[j] = line[i];
            j++;
        }
        i++;
    }
    new[j] = '\0';
    return new;
}

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int strongest_operator(t_db *db, t_parnth *head, char *line)
{
    int     i;
    t_op_node   tmp;
    t_op_node   strongest;

    i = 0;
    strongest.priority = -1;
    strongest.childs = NULL;
    strongest.op_presentation = NOT_FOUND;
    strongest.type = NOT_FOUND;
    strongest.origin = NULL;
    while (line[i])
    {
        tmp.op_presentation = is_op(line, &i);
        if (tmp.op_presentation != INVALID)
        {
            tmp.priority = priority_of_op(tmp.op_presentation);
            if (!is_inside_quotes(db, i) && !is_inside_paranthesis(head, i)
                && ((tmp.priority <= strongest.priority && tmp.priority != -1)
                || strongest.priority == -1))
                strongest = tmp;
        }
        i++;
    }
    return strongest.op_presentation;
}

int count_between_op(t_db *db, t_parnth *head, char *line, int op)
{
    int i;
    int counter;
    int reminder;

    i = 0;
    reminder = 0;
    counter = 0;
    while (line[i])
    {
        if (is_op(line, &i) == op && !is_inside_quotes(db, i) && !is_inside_paranthesis(head, i))
        {
            counter++;
            reminder = i;
        }
        i++;
    }
    if (reminder < i)
        counter++;
    return counter;
}
