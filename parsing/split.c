#include "../includes/main.h"
#include "../includes/parsing.h"

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

int smart_split(t_db *db, char *line, void **current_node, void *parent)
{
    t_parnth    *current_line_paranthesis;
    char        **splitted;
    int         op;
    int         i;

    current_line_paranthesis = NULL;
    if (track_paranthesis(db, &current_line_paranthesis, line) == FAILURE) return FAILURE;
    op = strongest_operator(db, current_line_paranthesis, line);
    if (current_line_paranthesis && op == NOT_FOUND)
        smart_split(db, remove_paranthesis(db, line, current_line_paranthesis), current_node, parent);
    else if (op != NOT_FOUND)
    {
        create_op_node(db, op, current_node, parent);
        ((t_op_node *)*current_node)->childs = gc_malloc(db, sizeof(void *) * count_between_op(db, current_line_paranthesis, line, op));
        ((t_op_node *)*current_node)->n_childs = count_between_op(db, current_line_paranthesis, line, op);
        splitted = split_line(db, line, ((t_op_node *)*current_node), current_line_paranthesis);
        i = 0;
        while (i < ((t_op_node *)*current_node)->n_childs)
        {
            ((t_op_node *)*current_node)->childs[i] = NULL;
            smart_split(db, splitted[i], &((t_op_node *)*current_node)->childs[i], *current_node);
            i++;
        }
    }
    else
    {
        PASS;
    }
    gc_free(db, current_line_paranthesis);
    return SUCCESS;
}
