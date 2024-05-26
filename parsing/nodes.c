#include "../includes/main.h"
#include "../includes/parsing.h"

// int add_op(t_db *db, char *op, int idx, int op_presentation)
// {
//     t_op_node   *new;
//     void        *tmp;

//     new = gc_malloc(db, sizeof(t_op_node));
//     if (!new) return (FAILURE);
//     new->type = OP_NODE;
//     new->childs = NULL;
//     new->n_childs = 0;

//     new->op_presentation = op_presentation;
//     if (op_presentation == OR)
//     {
//         if (db->root_node == NULL || db->and_count == 0)
//         {
//             tmp = db->root_node;
//             db->root_node = new;
//             new->childs = tmp;
//         }
//         return (SUCCESS);
//     }
//     else
//     {
        
//     }
// }

// void    create_cmd(t_)