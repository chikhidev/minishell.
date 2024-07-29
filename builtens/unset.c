#include "main.h"
#include "builtens.h"

void unset(t_db *db,    char    *args[])
{
    int i;

    i = 1;
    while (args[i])
    {
        del_env_node(&db->env_list, args[i]);
        del_exp_node(&db->exp_list, args[i]);
        i++;
    }
}