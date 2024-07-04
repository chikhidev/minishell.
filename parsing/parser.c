#include "../includes/main.h"
#include "../includes/parsing.h"

void print_nodes(void *head_node, int level)
{
    if (!head_node) return ;

    for (int i = 0; i < level; i++)
    {
        printf("   ");
    }
    if (((t_cmd_node *)head_node)->type == CMD_NODE)
    {
        printf("CMD_NODE: ");
        for (int i = 0; (((t_cmd_node *)head_node)->args[i]); i++)
        {
            printf("[%s] ", ((t_cmd_node *)head_node)->args[i]);
        }
        printf("\n");
        return ;
    }
    else if (((t_op_node *)head_node)->type == OP_NODE)
    {
        printf("OP_NODE: ");
        if (((t_op_node *)head_node)->op_presentation == OR)
            printf("OR\n");
        else if (((t_op_node *)head_node)->op_presentation == AND)
            printf("AND\n");
        else if (((t_op_node *)head_node)->op_presentation == PIPE)
            printf("PIPE\n");
        else if (((t_op_node *)head_node)->op_presentation == REDIR)
            printf("REDIR\n");
        else if (((t_op_node *)head_node)->op_presentation == APPEND)
            printf("APPEND\n");
        else if (((t_op_node *)head_node)->op_presentation == INPUT)
            printf("INPUT\n");
        else if (((t_op_node *)head_node)->op_presentation == HEREDOC)
            printf("HEREDOC\n");
    }
        
    for (int i = 0; i < ((t_op_node *)head_node)->n_childs; i++)
    {
        print_nodes(((t_op_node *)head_node)->childs[i], level + 1);
    }
}

/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/
int parser(t_db *db, char *line)
{
    t_parnth    *paranthesis;
    t_quote     *quotes;
    int         i;

    paranthesis = NULL;
    quotes = NULL;
    i = 0;
    if (ft_strlen(line) == 0) return (SUCCESS);
    skip_spaces(line, &i);
    if (line[i] == '\0') return (SUCCESS);
    if (track_quotes(db, &quotes, line) == FAILURE) return (FAILURE);
    if (track_paranthesis(db, &paranthesis, line, quotes) == FAILURE) return (FAILURE);
    if (track_operators(db, line) == FAILURE) return (FAILURE);
    // if (expand(db, &line, quotes) == FAILURE) return (FAILURE); let it be while splitting the commands
    if (smart_split(db, line, &db->root_node, NULL) == FAILURE) return (FAILURE);

    // // DEBUG --------------------------------------------------------
    printf(MAGENTA"\n[DEBUG] line: %s\n"RESET, line);

    t_op_node *head_node = db->root_node;
    print_nodes(head_node, 0);
    
    return (SUCCESS);
}
