#include "../includes/main.h"
#include "../includes/parsing.h"

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n');
}

int strongest_operator(t_db *db, char *line)
{
    int     i;
    t_op_node   tmp;
    t_op_node   strongest;

    (void)db;
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
            if (!is_inside_quotes(db, i) && !is_inside_paranthesis(db, i)
                && ((tmp.priority <= strongest.priority && tmp.priority != -1)
                || strongest.priority == -1))
                strongest = tmp;
        }
        i++;
    }
    return strongest.op_presentation;
}

int smart_split(t_db *db, char *line)
{
    // t_parnth    *current_line_paranthesis;
    int         op;


    op = strongest_operator(db, line);
    if (db->paranthesis && op == NOT_FOUND)
    {
        printf("should remove paranthesis and later split it\n");
    }
    else if (op != NOT_FOUND)
    {
        printf("should split using the operator\n");
    }
    else
    {
        printf("no operator found\n");
    }
    return SUCCESS;
}