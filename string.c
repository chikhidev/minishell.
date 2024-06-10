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

char *remove_paranthesis(t_db *db, char *line, t_parnth *local_paranths)
{
    char *new;
    int i;
    int j;

    new = gc_malloc(db, ft_strlen(line) - 1);
    i = 0;
    j = 0;
    while (line[i])
    {
        if (!(i == local_paranths->open_ || i == local_paranths->close_))
        {
            new[j] = line[i];
            j++;
        }
        i++;
    }
    new[j] = '\0';
    gc_free(db, line);
    return new;
}