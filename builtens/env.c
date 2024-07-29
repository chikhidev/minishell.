#include "builtens.h"
#include "main.h"

void    env(t_db *db)
{
    t_env_list  *curr;

    curr = db->env_list;
    while (curr)
    {
        printf("%s=%s\n", curr->key, curr->val);
        curr = curr->next;
    }
}