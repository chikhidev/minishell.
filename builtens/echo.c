#include "main.h"

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

void echo(char *args[], int n_args)
{
    int i;
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
        printf("%s", args[i]);
        i++;
        if (args[i])
            printf(" ");
    }
    if (new_line)
        printf("\n");
}