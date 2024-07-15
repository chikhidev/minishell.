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

    res = gc_malloc(db, sizeof(char) * (i - j + 1));
    if (!res)
        return NULL;
    ft_strlcpy(res, line + j, i - j + 1);
    return res;
}


BOOL is_the_first(char *line, t_tracker *tracker, int op)
{
    int i;

    i = 0;
    skip_spaces(line, &i);
    if (is_op(line, &i) == op && !is_inside_quotes(tracker->quotes, i) && !is_inside_paranthesis(tracker->paranthesis, i))
        return TRUE;
    return FALSE;
}