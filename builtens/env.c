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
        {
            ft_write(db, STDOUT_FILENO, curr->key, ft_strlen(curr->key));
            ft_write(db, STDOUT_FILENO, "=", 1);
            ft_write(db, STDOUT_FILENO, curr->val, ft_strlen(curr->val));
            ft_write(db, STDOUT_FILENO, "\n", 1);
        }
        curr = curr->next;
    }
    ft_write(db, STDOUT_FILENO, _key, ft_strlen(_key));
    ft_write(db, STDOUT_FILENO, "=", 1);
    ft_write(db, STDOUT_FILENO, _val, ft_strlen(_val));
    ft_write(db, STDOUT_FILENO, "\n", 1);
    return (0);
}