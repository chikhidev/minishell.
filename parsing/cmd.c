#include "../includes/main.h"
#include "../includes/parsing.h"

typedef struct s_iterators
{
    int i;
    int j;
} t_iterators;

// "command " " arg 1 " " arg 2 "
char **cmd_split(t_db *db, char *line, t_quote *quotes)
{
    char        **splitted;
    int         size;
    int         len;
    t_iterators it;

    len = (int)ft_strlen(line);
    size = count_args(line, quotes, len);
    splitted = gc_malloc(db, sizeof(char *) * (size + 1));
    if (!splitted)
        return NULL;
    ft_bzero(splitted, sizeof(char *) * (size + 1));
    ft_bzero(&it, sizeof(t_iterators));
    skip_spaces(line, &it.i);
    while (it.i < len)
    {
        if (is_whitespace(line[it.i]) && is_inside_quotes(quotes, it.i))
        {
            if (append_split(splitted, sub(db, line, it.i, it.j)) == FAILURE)
                return NULL;
            skip_spaces(line, &it.i);
            it.j = it.i;
        }
        it.i++;
    }
    if (append_split(splitted, sub(db, line, it.i, it.j)) == FAILURE)
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
