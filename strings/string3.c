#include "main.h"
#include "parsing.h"

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

int append_split(char **splitted, char *string)
{
    int i;

    if (!string)
        return FAILURE;
    i = 0;
    while (splitted[i])
        i++;
    splitted[i] = string;
    return SUCCESS;
}

char *sub(t_db *db, char *line, int i, int j)
{
    char *res;

    res = gc_malloc(db, sizeof(char) * (j - i + 1));
    if (!res)
        return NULL;
    ft_strlcpy(res, line + i, j - i + 1);
    return res;
}

/**
 * @details This function checks if the string is all whitespaces, pay attention that it frees the original string
 */
char *whithout_quotes(t_db *db, char *line)
{
    int i;
    char *res;
    int size;

    i = 0;
    size = ft_strlen(line);
    if (line[size - 1] == '\'' || line[size - 1] == '\"')
        size--;
    if (line[0] == '\'' || line[0] == '\"')
    {
        i++;
        size--;
    }
    res = gc_malloc(db, sizeof(char) * (size + 1));
    if (!res)
        return NULL;
    ft_strlcpy(res, line + i, size + 1);
    gc_free(db, line);
    return res;
}
