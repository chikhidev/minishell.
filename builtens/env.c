#include "builtens.h"
#include "parsing.h"
#include "string.h"
#include "main.h"

int    env_(t_db *db, char  *av[])
{
    char    *_key;
    char    *_val;
    t_env_list  *curr;

    if (count_array_len(av) > 1)
    {
        dprintf(2, "env: '%s': No such file or directory\n", av[1]);
        return 127;
    }
    curr = db->env_list;
    while (curr)
    {
        if (ft_strcmp(curr->key, "_") == 0)
        {
            _key = curr->key; 
            _val = curr->val; 
        }
        else
            printf("%s=%s\n", curr->key, curr->val);
        curr = curr->next;
    }
    printf("%s=%s\n", _key, _val);
    return (0);
}