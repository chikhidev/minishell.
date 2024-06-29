#include "../includes/main.h"
#include "../includes/parsing.h"

void print_nodes(t_op_node *head_node)
{
    t_op_node *tmp = head_node;
    
    if (tmp == NULL) return;
    printf("type-> %d, presentation-> %d, n_childs-> %d\n", tmp->type, tmp->op_presentation, tmp->n_childs);
    for (int i = 0; i < tmp->n_childs; i++)
    {
        print_nodes(tmp->childs[i]);
    }
}

/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/
int parser(t_db *db, char *line)
{
    int i;

    i = 0;
    if (ft_strlen(line) == 0) return (SUCCESS);
    skip_spaces(line, &i);
    if (line[i] == '\0') return (SUCCESS);
    if (track_quotes(db, line) == FAILURE) return (FAILURE);
    if (track_paranthesis(db, &db->paranthesis, line) == FAILURE) return (FAILURE);
    if (track_operators(db, line) == FAILURE) return (FAILURE);
    if (expand(db, &line) == FAILURE) return (FAILURE);
    if (smart_split(db, line, &db->root_node) == FAILURE) return (FAILURE);

    // // DEBUG --------------------------------------------------------
    printf(MAGENTA"\n[DEBUG] line: %s\n"RESET, line);

    t_op_node *head_node = db->root_node;
    print_nodes(head_node);
    
    return (SUCCESS);
}
