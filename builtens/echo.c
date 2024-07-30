#include "main.h"
#include "parsing.h"
#include "string.h"

bool has_new_line(char   *arg)
{
    int i;

    if (!arg || arg[0] != '-')
        return true;
    i = 1;
    while (arg[i])
    {
        if (arg[i] != 'n')
            return true;
        i++;
    }
    return false;
}

void echo(t_db  *db, char *args[], int n_args)
{
    int i;
    char    *token;
    bool new_line;

    new_line = true;
    if (n_args > 1 && !has_new_line(args[1]))
    {
        new_line = false;
        i = 2;
    }
    else
        i = 1;
    while (args[i])
    {
        token = whithout_quotes_ec(db, args[i]);
        if (!token)
            return (ec_void(db)); // make sute
        printf("%s", token);
        i++;
        if (token)
            printf(" ");
    }
    if (new_line)
        printf("\n");
}