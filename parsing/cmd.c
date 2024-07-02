#include "../includes/main.h"
#include "../includes/parsing.h"

// "command " " arg 1 " " arg 2 "
char **cmd_split(t_db *db, char *line, t_quote *quotes)
{
    char **splitted;
    int len;
    int j;
    int i;

    len = ft_strlen(line);
    splitted = gc_malloc(db, sizeof(char *) * (count_args(line, quotes, len) + 1));
    splitted[0] = NULL;
    printf("size of splitted: %d\n", (count_args(line, quotes, len) + 1));
    if (!splitted)
        return NULL;
    i = 0;
    j = 0;
    skip_spaces(line, &i);
    while (i < len)
    {
        if (is_whitespace(line[i]) && is_inside_quotes(quotes, i))
        {
            if (append_split(splitted, sub(db, line, i, j)) == FAILURE)
                return NULL;
            skip_spaces(line, &i);
            j = i;
        }
        i++;
    }
    if (append_split(splitted, sub(db, line, i, j)) == FAILURE)
        return NULL;

    return splitted;
}

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
