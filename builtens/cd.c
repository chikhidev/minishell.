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
    printf("cwd -> %s\n", getcwd(cwd, 200));
    if (count_array_len(args) > 2)
        printf("cd : too many arguments");
    else if (count_array_len(args) == 1)
    {
        home = get_environment_var(db, "HOME", db->env);
        if (home == NULL)
            printf("cd: home is not set");
        else
        {
            if (chdir(home) == -1)
                perror("cd");
        }
    }
    else
    {
        to = opendir(args[1]);
        if (to == NULL)
            return perror(args[1]);
        chdir(args[1]);
    }
}