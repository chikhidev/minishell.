#include "../includes/main.h"
#include "../includes/parsing.h"

int create_operator(t_db *db, int i, char *name)
{
    t_operators *op;
    t_operators *tmp;
    printf("creating -> %s\n", name);
    op = malloc(sizeof(t_operators));
    if (!op)
        return (FAILURE);
    op->i = i;
    op->name = name;
    op->size = ft_strlen(name);
    op->next = NULL;
    if (!db->ops)
        db->ops = op;
    else
    {
        tmp = db->ops;
        while (tmp->next)
            tmp = tmp->next;
        tmp->next = op;
    }
    return (SUCCESS);
}

int track_operators(t_db *db, char  *line)
{
    t_operators *ops;
    int i;

    i = 0;
    while (line[i])
    {
        printf("checking -> %c\n", line[i]);
        if (line[i] == '&' && line[i + 1] == '&')
            create_operator(db, i, "&&");
        else if (line[i] == '|' && line[i + 1] == '|')
            create_operator(db, i, "||");
        else if (line[i] == '<')
            create_operator(db, i, "<");
        else if (line[i] == '>')
            create_operator(db, i, ">");
        else if (line[i] == '|')
            create_operator(db, i, "|");
        i++;
    }
    i = 0;
    ops = db->ops;
    while (ops)
    {
        printf("op -> %s\n   inde -> %i\n", ops->name, ops->i);
        ops = ops->next;
    }
    return (SUCCESS);
}

int is_valid_op(char c, char next_c)
{
    if (c == '&' && next_c && next_c == '&')
        return (TRUE);
    else if (c == '|' && next_c && next_c == '|')
        return (TRUE);
    else if (c == '|' || c == '>' || c == '<')
        return (TRUE);
    return (FALSE);
}

int unused_ops(char *line)
{
    int i;

    i = 0;
    while (line[i] && is_whitespace(line[i]))
        i++;
    if (is_valid_op(line[i], line[i + 1]))
        return (TRUE);

    i = 0;
    // while (line[i])
    // {
    //     if (is_valid_op(line[i], line[i + 1], &i))
    //     {
    //         skip_spaces(line, &i);
    //         if (!line[i])
    //             return (TRUE);
    //     }
    //     i++;
    // }
    return (FALSE);
}
