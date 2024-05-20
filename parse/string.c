#include "../includes/main.h"

int is_white_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

char *concat_str(t_root *root, char *buffer, char _char_)
{
    char    *tmp;
    char    *current;

    current = malloc(sizeof(char) * 2);
    if (!current)
    {
        error("malloc failed", root);
        return (NULL);
    }
    current[0] = _char_;
    current[1] = '\0';
    tmp = ft_strjoin(buffer, current);
    if (!tmp)
    {
        free(current);
        error("malloc failed", root);
        return (NULL);
    }
    free(current);
    return tmp;
}