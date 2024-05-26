#include "../includes/main.h"
#include "../includes/parsing.h"

int add_op(t_db *db, char *op, int idx, int op_presentation)
{
    t_op_node   *new;

    new = gc_malloc(db, sizeof(t_op_node));
    if (!new) return (FAILURE);
    new->type = OP_NODE;
    new->childs = NULL;
    new->n_childs = 0;

    new->op_presentation = operator_priority(op);
    if (new->op_presentation == -1) return (FAILURE);
    if (new->op_presentation == AND_OR_PRIORITY)
    {
        if (db->root_node == NULL)
        {
            db->root_node = new;
            return (SUCCESS);
        }
        // else if ((t_op_node *)db->root_node->type == CMD_NODE
        //     && ((t_cmd_node *)db->root_node)->op_presentation == new->op_presentation)
    }
}

// void    create_cmd(t_)