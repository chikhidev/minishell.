#include "main.h"
#include "parsing.h"
#include "string.h"

bool    is_new_line_op(char *arg)
{
    int i;

    i = 1;
    if (!arg[0] || arg[0] != '-')
        return false;
    if (!arg[1])
        return false;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return false;
        i++;
    }
    return true;
}

int get_start_idx(char   **args, bool    *new_line)
{
    int i;
    int skip;
    i = 1;
    *new_line = true;
    skip = 1;
    while (args[i])
    {
        if (is_new_line_op(args[i]))
        {
            skip++;
            *new_line = false;
        }
        if (!is_new_line_op(args[i]))
            return skip;
        i++;
    }
    return skip;
}

int echo_(t_db  *db, char *args[])
{
    int i;
    bool new_line;
    char    *token;
    int n_args;
    (void)db;
    n_args = count_array_len(args);
    i = get_start_idx(args, &new_line);
    while (i < n_args)
    {
        token = whithout_quotes_ec(db, args[i]);
        if (!token)
            return (ec_void(db), 1); // make sute
        printf("%s", token);
        i++;
        if (i != n_args)
            printf(" ");
    }
    if (new_line)
        printf("\n");
    return (0);
}