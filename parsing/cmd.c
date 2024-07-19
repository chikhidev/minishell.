#include "main.h"
#include "parsing.h"

int    create_cmd_node(t_db *db, void **current_node, void *parent)
{
    *current_node = gc_malloc(db, sizeof(t_cmd_node));
    if (!*current_node)
        return FAILURE;
    (CURR_CMD)->origin = parent;
    (CURR_CMD)->type = CMD_NODE;
    (CURR_CMD)->cmd_path = NULL;
    (CURR_CMD)->args = NULL;
    /*to be changed later*/
    (CURR_CMD)->input_fd = STDIN_FILENO;
    (CURR_CMD)->output_fd = STDOUT_FILENO;
    return SUCCESS;
}

int count_op(char *line, int op)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (line[i])
    {
        if (is_op(line, &i) == op)
            count++;
        i++;
    }
    return count;
}
