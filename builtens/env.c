#include "builtens.h"
#include "parsing.h"
#include "string.h"
#include "main.h"

int    env(t_db *db, char  *av[])
{
    int args_len;
    t_env_list  *curr;

    args_len = count_array_len(av);
    if (args_len != 1)
        return (printf("env: '%s': No such file or directory\n", av[1]), FAILURE);
    else
    {

        curr = db->env_list;
        while (curr)
        {
            printf("%s=%s\n", curr->key, curr->val);
            curr = curr->next;
        }
    }
    return (SUCCESS);
}