#include "../includes/main.h"
#include "../includes/parsing.h"

int is_whitespace(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

int count_words(char *str)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (str[i])
    {
        if (!is_whitespace(str[i]))
        {
            count++;
            while (str[i] && !is_whitespace(str[i]))
                i++;
        }
        else
            i++;
    }
    return (count);
}

void    *free_failed_split(char **res)
{
    int i;

    i = 0;
    while (res[i])
    {
        free(res[i]);
        i++;
    }
    free(res);
    return (NULL);
}

char    **whitespaces_split(t_db *db, char *str, int start)
{
    char    **res;
    char    *tmp;
    int     i;
    int     j;

    res = gc_malloc(db, sizeof(char *) * (count_words(str) + 1));
    if (!res)
        return (NULL);
    res[0] = NULL;
    i = 0;
    j = 0;
    while (str[i])
    {
        if (is_whitespace(str[i]) && !is_inside_quotes(db, start + i))
        {
            tmp = gc_malloc(db, i + 1);
            if (!tmp)
                return (free_failed_split(res));
            ft_strlcpy(tmp, str, i + 1);
            res[j++] = tmp;
            res[j] = NULL;
            while (str[i] && is_whitespace(str[i]))
                i++;
            if (!str[i])
                break;
            str += i;
            i = 0;
        }
        i++;
    }
    if (str[i - 1] && !is_whitespace(str[i - 1]))
    {
        tmp = gc_malloc(db, i + 1);
        if (!tmp)
            return (free_failed_split(res));
        ft_strlcpy(tmp, str, i + 1);
        res[j++] = tmp;
        res[j] = NULL;
    }

    return (res);
}
