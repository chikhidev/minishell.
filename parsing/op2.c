#include "../includes/main.h"
#include "../includes/parsing.h"

int is_op(char *line, int *i)
{
    if (line[*i] == '&' && line[*i + 1] && line[++(*i)] == '&')
        return AND;
    else if (line[*i] == '|' && line[*i + 1] && line[(*i) + 1] == '|')
        return (++(*i), OR);
    else if (line[*i] == '>' && line[*i + 1] && line[(*i) + 1] == '>')
        return (++(*i), APPEND);
    else if (line[*i] == '<' && line[*i + 1] && line[(*i) + 1] == '<')
        return (++(*i), HEREDOC);
    else if (line[*i] == '|')
        return PIPE;
    else if (line[*i] == '>')
        return REDIR;
    else if (line[*i] == '<')
        return INPUT;
    return INVALID;
}

int priority_of_op(int op)
{
    if (op == AND || op == OR) return AND_OR_PRIORITY;
    else if (op == PIPE) return PIPE_PRIORITY;
    else if (op == REDIR || op == APPEND) return REDIR_PRIORITY;
    else if (op == HEREDOC) return REDIR_PRIORITY;
    return NOT_FOUND;
}

void    create_op_node(t_db *db, int op, void **current_node, void *parent)
{
    *current_node = gc_malloc(db, sizeof(t_op_node));
    ((t_op_node *)*current_node)->origin = parent;
    ((t_op_node *)*current_node)->type = OP_NODE;
    ((t_op_node *)*current_node)->priority = priority_of_op(op);
    ((t_op_node *)*current_node)->op_presentation = op;
}
