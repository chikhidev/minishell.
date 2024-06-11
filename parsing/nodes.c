#include "../includes/main.h"
#include "../includes/parsing.h"

int insert_op(t_db *db, t_op_node *op)
{
    t_op_node *tmp;
    int     i;

    if (!db->root_node)
    {
        db->root_node = op;
        return (SUCCESS);
    }
    
    // else we should find the right place to insert the operator

    return (SUCCESS);
}