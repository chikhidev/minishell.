#include "builtens.h"
#include "parsing.h"
#include "string.h"
#include "main.h"

int    env_(t_db *db, char  *av[])
{
    char    *_key;
    char    *_val;
    (void)av;
    t_env_list  *curr;

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