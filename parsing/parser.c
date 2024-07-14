#include "../includes/main.h"
#include "../includes/parsing.h"

void print_nodes(void *node, int level)
{
    if (!node) return ;

    for (int i = 0; i < level; i++)
    {
        printf("   ");
    }
    if (CMD->type == CMD_NODE && CMD->args)
    {
        printf("CMD_NODE: ");
        for (int i = 0; (CMD->args[i]); i++)
        {
            printf("[%s] ", CMD->args[i]);
        }
        printf("\n");
        return ;
    }
    else if (OP->type == OP_NODE)
    {
        printf("OP_NODE: ");
        if (OP->op_presentation == OR)
            printf("OR\n");
        else if (OP->op_presentation == AND)
            printf("AND\n");
        else if (OP->op_presentation == PIPE)
            printf("PIPE\n");
        else if (OP->op_presentation == REDIR)
            printf("REDIR\n");
        else if (OP->op_presentation == APPEND)
            printf("APPEND\n");
        else if (OP->op_presentation == INPUT)
            printf("INPUT\n");
        else if (OP->op_presentation == HEREDOC)
        {
            printf("HEREDOC");
            if (OP->neighbour != NULL)
            {
                if (((t_op_node *)OP->neighbour)->type == CMD_NODE)
                {
                    for (int i = 0; ((t_cmd_node *)OP->neighbour)->args[i]; i++)
                    {
                        printf("[%s] ", ((t_cmd_node *)OP->neighbour)->args[i]);
                    }
                }
                else
                {
                    printf(MAGENTA"\n");
                    for (int i = 0; i < level; i++)
                    {
                        printf("   ");
                    }
                    if (((t_op_node *)OP->neighbour)->op_presentation == HEREDOC)
                        printf("HEREDOC\n");
                    else if (((t_op_node *)OP->neighbour)->op_presentation == REDIR)
                        printf("REDIR\n");
                    else if (((t_op_node *)OP->neighbour)->op_presentation == APPEND)
                        printf("APPEND\n");
                    else if (((t_op_node *)OP->neighbour)->op_presentation == INPUT)
                        printf("INPUT\n");
                    else if (((t_op_node *)OP->neighbour)->op_presentation == PIPE)
                        printf("PIPE\n");
                    else if (((t_op_node *)OP->neighbour)->op_presentation == AND)
                        printf("AND\n");
                    else if (((t_op_node *)OP->neighbour)->op_presentation == OR)
                        printf("OR\n");
                    
                    for (int i = 0; i < ((t_op_node *)OP->neighbour)->n_childs; i++)
                    {
                        print_nodes(((t_op_node *)OP->neighbour)->childs[i], level + 1);
                    }
                    printf(RESET);
                }
            }
            printf("\n");
        }
    }
        
    for (int i = 0; i < OP->n_childs; i++)
    {
        print_nodes(OP->childs[i], level + 1);
    }
}

/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/

int print_here_docs(t_here_doc *here_docs)
{
    t_here_doc *here_doc;

    here_doc = here_docs;
    while (here_doc)
    {
        here_doc = here_doc->next;
    }
    return (SUCCESS);
}

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

    t_op_node *node = db->root_node;
    print_nodes(node, 0);
    print_here_docs(db->here_docs);
    
    return (SUCCESS);
}
