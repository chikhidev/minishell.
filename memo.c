#include "includes/main.h"

void    free_memo(t_root *root)
{
    for (int i = 0; i < root->num_commands; i++)
    {
        free(root->commands[i]->cmd);
        for (int j = 0; root->commands[i]->args[j]; j++)
            free(root->commands[i]->args[j]);
        free(root->commands[i]->args);
        free(root->commands[i]);
    }
    free(root->commands);
    free(root);
}
