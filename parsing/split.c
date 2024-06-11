#include "../includes/main.h"
#include "../includes/parsing.h"

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

int is_all_whitespace(char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if (!is_whitespace(line[i])) return 0;
        i++;
    }
    return 1;
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
        if (counter == 0 && !is_op(line, &i) && !is_whitespace(line[i]))
        {
            counter++;
            reminder = i;
        }
        else if (is_op(line, &i) == op && !is_inside_quotes(db, i) && !is_inside_paranthesis(head, i))
        {
            counter++;
            reminder = i;
        }
        i++;
    }
    if (!is_all_whitespace(line + reminder + 1)) counter++;
    return counter;
}

int smart_split(t_db *db, char *line)
{
    t_parnth    *current_line_paranthesis;
    int         op;

    printf(GREEN"[DEBUG] smart split working on: %s\n"RESET, line);

    /* here just to update the paranthesis on the current line */
    current_line_paranthesis = NULL;
    if (track_paranthesis(db, &current_line_paranthesis, line) == FAILURE) return FAILURE;
    op = strongest_operator(db, current_line_paranthesis, line);
    if (current_line_paranthesis && op == NOT_FOUND)
    {
        printf("[DEBUG] should remove paranthesis and later split it\n");
        smart_split(db, remove_paranthesis(db, line, current_line_paranthesis));
    }
    else if (op != NOT_FOUND)
    {
        printf("[DEBUG] should split using the operator\n");
        printf("[DEBUG] count between op: %d\n", count_between_op(db, current_line_paranthesis, line, op));
    }
    else
    {
        printf("[DEBUG] no operator found\n");
    }
    /*get rid of i tbefore going to the next split*/
    gc_free(db, current_line_paranthesis);
    return SUCCESS;
}
