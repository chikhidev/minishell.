#include "main.h"
#include "parsing.h"
#include "builtens.h"

void print_nodes(t_db   *db, void *node, int level)
{
    if (!node) return ;

    for (int i = 0; i < level; i++)
    {
        printf("    ");
    }
    if (CMD->type == CMD_NODE)
    {
        printf("CMD_NODE: ");
        if (ft_strcmp(CMD->args[0], "echo") == 0)
            echo(CMD->args, 3);
        else if (ft_strcmp(CMD->args[0], "export") == 0)
            export(db, CMD->args);
        else if (ft_strcmp(CMD->args[0], "pwd") == 0)
            pwd(db);
        else if (ft_strcmp(CMD->args[0], "env") == 0)
            env(db);
        else if (ft_strcmp(CMD->args[0], "cd") == 0)
            cd(db, CMD->args);
        for (int i = 0; (CMD->args[i]); i++)
        {
            printf("[%s] ", CMD->args[i]);
        }
        printf("\n");
        for (int i = 0; i < level; i++)
        {
            printf("    ");
        }
        
        if (CMD->input_fd != STDIN_FILENO)
            printf("Input: %d ", CMD->input_fd);
        if (CMD->output_fd != STDOUT_FILENO)
            printf("Output: %d ", CMD->output_fd);

        printf("\n"RESET);
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
    }
    
    for (int i = 0; i < OP->n_childs; i++)
    {
        print_nodes(db, OP->childs[i], level + 1);
    }
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
    CATCH_ONFAILURE(
        syntax_checker(db, line, &i),
        FAILURE
    )
    if (line[i] == '\0') return (SUCCESS);
    if (track_quotes(db, &quotes, line) == FAILURE) return (FAILURE);
    if (track_paranthesis(db, &paranthesis, line, quotes) == FAILURE) return (FAILURE);
    if (track_operators(db, line) == FAILURE) return (FAILURE);
    // if (expand(db, &line, quotes) == FAILURE) return (FAILURE); let it be while splitting the commands
    if (smart_split(db, line, &db->root_node, NULL) == FAILURE) return (FAILURE);

    // // DEBUG --------------------------------------------------------

    t_op_node *node = db->root_node;
    print_nodes(db, node, 0);
    
    return (SUCCESS);
}
