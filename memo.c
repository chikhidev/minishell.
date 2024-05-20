#include "includes/main.h"

void    free_memo(t_root *root)
{
    printf("entered free_memo\n");
    for (int i = 0; i < root->num_commands; i++)
    {
        free(root->commands[i]->cmd);
        if (root->commands[i]->args)
        {
            for (int j = 0; root->commands[i]->args[j]; j++)
                free(root->commands[i]->args[j]);
        }
        free(root->commands[i]->args);
        free(root->commands[i]);
    }
    free(root->commands);
    printf("after\n");
    root->commands = NULL;
    for (int i = 0; root->cmds[i] && root->cmds; i++)
        free(root->cmds[i]);
    free(root->cmds);
    root->cmds = NULL;
    free(root->buffer);
    root->buffer = NULL;
    free(root->tmp);
    root->tmp = NULL;
    free(root->quotes);
    root->quotes = NULL;
}

void    *ft_realloc(void *ptr, size_t size)
{
    void    *new_ptr;

    new_ptr = malloc(size);
    if (!new_ptr)
        return (NULL);
    if (ptr)
    {
        ft_memcpy(new_ptr, ptr, size);
        free(ptr);
    }
    return (new_ptr);
}
