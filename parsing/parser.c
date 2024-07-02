#include "../includes/main.h"
#include "../includes/parsing.h"

void print_nodes(t_op_node *head_node, int level)
{
    t_op_node *tmp = head_node;
    //print node as a tree

    if (!tmp) return;

    for (int i = 0; i < level; i++)
    {
        printf("  ");
    }
    if (tmp->type == CMD_NODE)
    {
        printf("CMD_NODE: %s\n", ((t_cmd_node *)tmp)->cmd_path);
    }
    else if (tmp->type == OP_NODE)
    {
        printf("OP_NODE: ");
        if (tmp->op_presentation == OR)
            printf("OR\n");
        else if (tmp->op_presentation == AND)
            printf("AND\n");
        else if (tmp->op_presentation == PIPE)
            printf("PIPE\n");
        else if (tmp->op_presentation == REDIR)
            printf("REDIR\n");
        else if (tmp->op_presentation == APPEND)
            printf("APPEND\n");
        else if (tmp->op_presentation == INPUT)
            printf("INPUT\n");
        else if (tmp->op_presentation == HEREDOC)
            printf("HEREDOC\n");
    }
        
    for (int i = 0; i < tmp->n_childs; i++)
    {
        print_nodes(tmp->childs[i], level + 1);
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
    if (smart_split(db, line, &db->root_node, NULL) == FAILURE) return (FAILURE);

    // // DEBUG --------------------------------------------------------
    printf(MAGENTA"\n[DEBUG] line: %s\n"RESET, line);

    t_op_node *head_node = db->root_node;
    print_nodes(head_node, 0);
    
    return (SUCCESS);
}
