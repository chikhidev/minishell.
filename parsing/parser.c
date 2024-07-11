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
            printf("----> ");
            if (OP->neighbour != NULL)
            {
                t_cmd_node *cmd = OP->neighbour;
                for (int i = 0; cmd->args && (cmd->args[i]); i++)
                {
                    printf("[%s] ", cmd->args[i]);
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
    printf(MAGENTA"\n[DEBUG] line: %s\n"RESET, line);

    t_op_node *node = db->root_node;
    print_nodes(node, 0);
    print_here_docs(db->here_docs);
    
    return (SUCCESS);
}
