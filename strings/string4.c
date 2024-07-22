#include "main.h"
#include "parsing.h"

int find_c(char *str, char  c)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == c)
            return (i);
        i++;
    }
    return -1;
}

int length_til(char *str, char stop)
{
    int i;

    i = 0;
    while (str[i])
    {
        if (str[i] == stop)
            return i;
        i++;
    }
    return -1;
}