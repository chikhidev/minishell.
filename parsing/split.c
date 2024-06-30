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


int is_op2(char *line, int *i)
{
    if (line[*i] == '&' && line[*i + 1] && line[(*i) + 1] == '&')
        return AND;
    else if (line[*i] == '|' && line[*i + 1] && line[(*i) + 1] == '|')
        return (OR);
    else if (line[*i] == '>' && line[*i + 1] && line[(*i) + 1] == '>')
        return (APPEND);
    else if (line[*i] == '<' && line[*i + 1] && line[(*i) + 1] == '<')
        return (HEREDOC);
    else if (line[*i] == '|')
        return PIPE;
    else if (line[*i] == '>')
        return REDIR;
    else if (line[*i] == '<')
        return INPUT;
    return INVALID;
}

void skip_op(int *i, char *line)
{
    if (line[*i] == '&' && line[*i + 1] && line[++(*i)] == '&')
        ((*i) += 2);
    else if (line[*i] == '|' && line[*i + 1] && line[(*i) + 1] == '|')
        ((*i) += 2);
    else if (line[*i] == '>' && line[*i + 1] && line[(*i) + 1] == '>')
        ((*i) += 2);
    else if (line[*i] == '<' && line[*i + 1] && line[(*i) + 1] == '<')
        ((*i) += 2);
    else if (line[*i] == '|')
        (*i)++;
    else if (line[*i] == '>')
        (*i)++;
    else if (line[*i] == '<')
        (*i)++;
    return;
}

char **split_line(t_db * db, char *line, t_op_node *op, t_parnth *head)
{
    int i;
    int len;
    int k;
    char **splitted;

    i = 0;
    len = 0;
    k = 0;

    splitted = gc_malloc(db, sizeof(char *) * op->n_childs);
    while (line[i])
    {
        if (is_op2(line, &i) == op->op_presentation && !is_inside_quotes(db, i) && !is_inside_paranthesis(head, i))
        {
            splitted[k] = gc_malloc(db, sizeof(char) * (len + 1));
            ft_strlcpy(splitted[k], line + i - len, len + 1);
            skip_op(&i, line);
            k++;
            len = 0;
        }
        len++;
        i++;
    }

    if (len > 0)
    {
        splitted[k] = gc_malloc(db, sizeof(char) * (len + 1));
        ft_strlcpy(splitted[k], line + i - len, len + 1);

    }
    return splitted;
}

int smart_split(t_db *db, char *line, void **parent)
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
        smart_split(db, remove_paranthesis(db, line, current_line_paranthesis), parent);
    }
    else if (op != NOT_FOUND)
    {
        printf("[DEBUG] should split using the operator %d\n", op);
        printf("[DEBUG] count between op: %d\n", count_between_op(db, current_line_paranthesis, line, op));

        t_op_node *new_node = gc_malloc(db, sizeof(t_op_node));
        if (!new_node)
        {
            printf("Error: malloc failed\n");
            return FAILURE;
        }
        new_node->origin = NULL;
        new_node->type = OP_NODE;
        new_node->priority = priority_of_op(op);
        new_node->op_presentation = op;
        new_node->childs = gc_malloc(db, sizeof(void *) * count_between_op(db, current_line_paranthesis, line, op));
        new_node->n_childs = count_between_op(db, current_line_paranthesis, line, op);
        for (int i = 0; i < new_node->n_childs; i++)
        {
            new_node->childs[i] = NULL;
        }
        
        char **splitted = split_line(db, line, new_node, current_line_paranthesis);
        if (*parent == NULL)
        {
            *parent = new_node;
            for (int i = 0; i < new_node->n_childs; i++)
            {
                printf("spliited  -> %s\n", splitted[i]);
                smart_split(db, splitted[i], &new_node->childs[i]);
            }
        }
    }
    else
    {
        printf("[DEBUG] no operator found\n");
    }
    /*get rid of i tbefore going to the next split*/
    gc_free(db, current_line_paranthesis);
    return SUCCESS;
}
