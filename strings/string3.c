#include "../includes/main.h"
#include "../includes/parsing.h"

int count_args(char *line, t_quote *quotes, int len)
{
    int i;
    int reminder;
    int count;

    i = 0;
    count = 0;
    skip_spaces(line, &i);
    while (i < len)
    {
        reminder = i;
        while (line[i] && !is_whitespace(line[i]) && !is_inside_quotes(quotes, i))
            i++;
        if (reminder != i)
            count++;
        skip_spaces(line, &i);
    }
    return count;
}

int append_split(t_db *db, char **splitted, char *string)
{
    int i;

    if (!string)
        return FAILURE;
    i = 0;
    while (splitted[i])
        i++;
    splitted[i] = string;
    splitted[i + 1] = NULL;
    return SUCCESS;
}

char *sub(t_db *db, char *line, int i, int j)
{
    char *res;

    res = gc_malloc(db, sizeof(char) * (i - j + 1));
    if (!res)
        return NULL;
    ft_strlcpy(res, line + j, i - j + 1);
    return res;
}

// "command " " arg 1 " " arg 2 "
char **cmd_split(t_db *db, char *line, t_quote *quotes)
{
    char **splitted;
    int len;
    int j;
    int i;

    len = ft_strlen(line);
    splitted = gc_malloc(db, sizeof(char *) * (count_args(line, quotes, len) + 1));
    if (!splitted)
        return NULL;
    i = 0;
    j = 0;
    skip_spaces(line, &i);
    while (i < len)
    {
        if (is_whitespace(line[i]) || is_inside_quotes(quotes, i))
        {
            if (append_split(db, splitted, sub(db, line, i, j)) == FAILURE)
                return NULL;
            skip_spaces(line, &i);
            j = i;
        }
        i++;
    }
    return splitted;
}