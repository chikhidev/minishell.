#include "../includes/main.h"
#include "../includes/parsing.h"

int    create_cmd_node(t_db *db, void **current_node, void *parent)
{
    *current_node = gc_malloc(db, sizeof(t_cmd_node));
    if (!*current_node)
        return FAILURE;
    ((t_cmd_node *)*current_node)->origin = parent;
    ((t_cmd_node *)*current_node)->type = CMD_NODE;
    ((t_cmd_node *)*current_node)->cmd_path = NULL;
    return SUCCESS;
}
