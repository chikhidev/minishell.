#include "main.h"
#include "parsing.h"

int count_line_args(char *line, t_quote *quotes, int len)
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

    i = 1;
    size = ft_strlen(line);

    if (!(
        (line[0] == '\'' && line[size - 1] == '\'')
        || (line[0] == '\"' && line[size - 1] == '\"')
    ))
        return line;

    size -= 2;
    res = gc_malloc(db, sizeof(char) * (size + 1));
    if (!res)
        return NULL;
    ft_strlcpy(res, line + i, size + 1);
    gc_free(db, line);
    return res;
}

bool contains(char  *str, char    *sub)
{
    int i;
    int j;

    i = 0;
    while (str[i])
    {
        j = 0;
        while (str[i + j] == sub[j] && str[i + j] && sub[j])
            j++;
        if (!sub[j])
            return true;
        i++;
    }
    return false;
}