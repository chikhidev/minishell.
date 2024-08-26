#include "main.h"
#include "parsing.h"

int is_op(char *line, int *i)
{
    if (line[*i] == '|')
        return PIPE;
    return INVALID;
}

int priority_of_op(int op)
{
    if (op == AND || op == OR) return AND_OR_PRIORITY;
    else if (op == PIPE) return PIPE_PRIORITY;
    return NOT_FOUND;
}

int    create_op_node(t_db *db, int op, void **current_node)
{
    *current_node = gc_malloc(db, sizeof(t_op_node));
    ((t_op_node *)*current_node)->type = OP_NODE;
    ((t_op_node *)*current_node)->op_presentation = op;
    ((t_op_node *)*current_node)->n_childs = 0;
    ((t_op_node *)*current_node)->input_fd = STDIN_FILENO;
    ((t_op_node *)*current_node)->output_fd = STDOUT_FILENO;

    return (SUCCESS);
}
