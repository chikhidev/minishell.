#include "builtens.h"
#include "main.h"

void    env(t_db *db)
{
    // show export here;
    t_env_list  *vars;
    t_env_list  *curr;

    vars = db->env_list;
    curr = vars;
    while (curr)
    {
        printf("%s\n", curr->data);
        curr = curr->next;
    }
}