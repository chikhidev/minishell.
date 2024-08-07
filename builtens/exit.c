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
        return (FAILURE);
    }
    if (args_len == 2)
        exit_code = ft_atoi(av[1]);
    gc_void(db);
    ec_void(db);
    exit(exit_code);
}