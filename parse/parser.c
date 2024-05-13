#include "../includes/main.h"

void    parse(char *line, t_root *root)
{
    int     i;

    i = 0;
    root = malloc(sizeof(t_root));
    if (!root)
        return ;
    while (line[i])
    {
        if (line[i] == ' ')
        i++;
    }
}
