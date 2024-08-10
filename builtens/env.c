#include "builtens.h"
#include "parsing.h"
#include "string.h"
#include "main.h"

int    env_(t_db *db, char  *av[])
{
    (void)av;
    t_env_list  *curr;

    curr = db->env_list;
    while (curr)
    {
        printf("%s=%s\n", curr->key, curr->val);
        curr = curr->next;
    }
    return (0);
}