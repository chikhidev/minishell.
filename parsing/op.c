#include "../includes/main.h"
#include "../includes/parsing.h"

int is_valid_op(char c, char next_c, int *i)
{
    if (c == '&' && next_c && next_c == '&')
    {
        *i += 1;
        return (TRUE);
    }
    else if (c == '|' && next_c && next_c == '|')
    {
        *i += 1;
        return (TRUE);
    }
    else if (c == '|' || c == '>' || c == '<')
        return (TRUE);
    return (FALSE);
}

int unused_ops(char *line)
{
    int i;

    i = 0;
    while (line[i] && is_whitespace(line[i]))
        i++;
    if (is_valid_op(line[i], line[i + 1], &i))
        return (TRUE);

    i = 0;
    // while (line[i])
    // {
    //     if (is_valid_op(line[i], line[i + 1], &i))
    //     {
    //         skip_spaces(line, &i);
    //         if (!line[i])
    //             return (TRUE);
    //     }
    //     i++;
    // }
    return (FALSE);
}
