#include "includes/main.h"
#include "includes/parsing.h"

char    *concat(t_db *db, char *s, char single_char)
{
    char    *new;
    int     i;

    new = gc_malloc(db, ft_strlen(s) + 2);
    if (!new) return (NULL);
    i = 0;
    while (s && s[i])
    {
        new[i] = s[i];
        i++;
    }
    new[i] = single_char;
    new[i + 1] = '\0';
    return (new);
}

int operator_priority(char *op)
{
    if (ft_strncmp(op, "&&", 2) == 0
        || ft_strncmp(op, "||", 2) == 0)
        return (AND_OR_PRIORITY);
    if (ft_strncmp(op, ">", 1) == 0
        || ft_strncmp(op, ">>", 2) == 0
        || ft_strncmp(op, "<", 1) == 0)
        return (REDIR_PRIORITY);
    if (ft_strncmp(op, "|", 1) == 0)
        return (PIPE_PRIORITY);
    return (-1);
}

/**
 * this function just skip the white-spaces
*/
void skip_spaces(char *line, int *i)
{
    while (line[*i] && (line[*i] == 32 || (line[*i] >= 9 && line[*i] <= 13)))
        (*i)++;
}

int count(char *line, char c)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (line[i])
    {
        if (line[i] == c)
            count++;
        i++;
    }
    return (count);
}

int are_all(char *str, int (*f)(int))
{
    int i;

    i = 0;
    while (str[i])
    {
        if (!f(str[i]))
            return (0);
        i++;
    }
    return (1);
}
