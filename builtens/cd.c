#include "main.h"
#include "builtens.h"
#include "parsing.h"
#include "string.h"



void cd(t_db    *db, char    *args[])
{
    (void)db;
    DIR    *to;
    char cwd[200];
    char *home;
    int    flag;
    t_exp_list *pwd_exp_node;
    t_env_list *pwd_env_node;
    printf("cwd -> %s\n", getcwd(cwd, 200));
    home = get_environment_var(db, "HOME", db->env);
    pwd_exp_node = get_exp_node(db->exp_list, "PWD");
    pwd_env_node = get_env_node(db->env_list, "PWD");
    if (count_array_len(args) > 2)
        printf("cd : too many arguments");
    else if (count_array_len(args) == 1)
    {
        if (home == NULL)
            printf("cd: home is not set");
        else
        {
            flag = chdir(home);
            if (flag == -1)
                perror("cd");
            if (pwd_env_node)
                pwd_env_node->val = ft_strdup(db, home);
            if (pwd_exp_node)
                pwd_exp_node->val = ft_strdup(db, home);
        }
    }
    else
    {
        to = opendir(args[1]);
        if (to == NULL)
            return perror(args[1]);
        flag = chdir(args[1]);
        if (flag == -1)
            perror("cd");
        if (pwd_env_node)
            pwd_env_node->val = ft_strdup(db, home);
        if (pwd_exp_node)
            pwd_exp_node->val = ft_strdup(db, home);
    }
}