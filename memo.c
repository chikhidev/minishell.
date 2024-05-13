#include "includes/main.h"

void    free_memo(t_root *root)
{
    if (!root)
        return ;
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

//> ls -l
//> ls "-l"
//> ls "-l -a"
//> ls -l > out
//> ls -l >> out
//> ls -l | grep .c | wc -l
//> ls -l | grep .c