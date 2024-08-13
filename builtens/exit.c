#include "main.h"
#include "builtens.h"
#include "string.h"

int exit_(t_db  *db,    char    *av[])
{
    unsigned    char    exit_code;
    int args_len;

    exit_code = 0;
    args_len = count_array_len(av);

    if (args_len > 2)
    {
        printf("exit: too many arguments\n");
        return (1);
    }
    if (args_len == 2)
        exit_code = ft_atoi(av[1]);

    error(db, NULL, "exit");
    ec_void(db);
    gc_void(db);
    if (db->stdin_dup != -1)
        close(db->stdin_dup);
    if (db->stdout_dup != -1)
        close(db->stdout_dup);
    exit(exit_code);
}