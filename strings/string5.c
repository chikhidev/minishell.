#include "main.h"
#include "parsing.h"

char *gc_join(t_db *db, char *s1, char *s2)
{
    char *new;
    int i;
    int j;

    if (!s1 && !s2)
        return (NULL);
    new = gc_malloc(db, ft_strlen(s1) + ft_strlen(s2) + 1);
    if (!new)
        return (NULL);
    i = 0;
    j = 0;
    while (s1 && s1[i])
    {
        new[j] = s1[i];
        i++;
        j++;
    }
    i = 0;
    while (s2 && s2[i])
    {
        new[j] = s2[i];
        i++;
        j++;
    }
    new[j] = '\0';
    return (new);
}


BOOL contains_spaces_btwn(char *s)
{
    int i;

    i = 0;
    while (s[i])
    {
        if (is_whitespace(s[i]))
        {
            skip_spaces(s, &i);
            if (!s[i])
                return (FALSE);
            if (s[i] && !is_whitespace(s[i]))
                return (TRUE);
        }
        i++;
    }
    return (FALSE);
}