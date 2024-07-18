#include "main.h"
#include "parsing.h"

int is_op(char *line, int *i)
{
    if (line[*i] == '&' && line[*i + 1] && line[++(*i)] == '&')
        return AND;
    else if (line[*i] == '|' && line[*i + 1] && line[(*i) + 1] == '|')
        return (++(*i), OR);
    else if (line[*i] == '|')
        return PIPE;
    return INVALID;
}

int priority_of_op(int op)
{
    if (op == AND || op == OR) return AND_OR_PRIORITY;
    else if (op == PIPE) return PIPE_PRIORITY;
    return NOT_FOUND;
}

int push_heredoc(t_db *db, t_op_node *here_doc) // here_doc is a t_op_node
{
    t_here_doc    *tmp;

    if (!db->here_docs)
    {
        db->here_docs = gc_malloc(db, sizeof(t_here_doc));
        if (!db->here_docs)
            return error(db, NULL, "Malloc failed");
        db->here_docs->ptr = here_doc;
        db->here_docs->next = NULL;
        return (SUCCESS);
    }
    tmp = db->here_docs;
    while (tmp->next)
        tmp = tmp->next;
    tmp->next = gc_malloc(db, sizeof(t_here_doc));
    if (!tmp->next)
        return (error(db, NULL, "Malloc failed"));
    tmp->next->ptr = here_doc;
    tmp->next->next = NULL;
    return (SUCCESS);
}

int    create_op_node(t_db *db, int op, void **current_node, void *parent)
{
    *current_node = gc_malloc(db, sizeof(t_op_node));
    ((t_op_node *)*current_node)->origin = parent;
    ((t_op_node *)*current_node)->type = OP_NODE;
    ((t_op_node *)*current_node)->priority = priority_of_op(op);
    ((t_op_node *)*current_node)->op_presentation = op;
    ((t_op_node *)*current_node)->n_childs = 0;

    return (SUCCESS);
}
