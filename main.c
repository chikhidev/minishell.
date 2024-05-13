#include "includes/main.h"

/**
 * @brief Print the root structure
 * @param root The root structure to print
*/
void    ft_print_root(t_root *root)
{
    for (int i = 0; i < root->num_commands; i++)
    {
        printf("cmd: %s\n", root->commands[i]->cmd);
        for (int j = 0; root->commands[i]->args[j]; j++)
            printf("args[%d]: %s\n", j, root->commands[i]->args[j]);
        printf("\n");
    }
}

int main(int ac, char **av, char **env)
{
    char    *line;
    t_root  *root;

    (void)ac;
    (void)av;
    (void)env;
    root = NULL;
    line = readline("@> ");
    while (line)
    {
        add_history(line);
        parse(line, root);
        free(line);
        line = readline("@> ");
    }
    return 0;
}
