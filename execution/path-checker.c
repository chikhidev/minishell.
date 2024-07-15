#include "main.h"
#include "exec.h"
#include "parsing.h"

BOOL    is_absolute_path(char *path)
{
    int i;

    i = 0;
    skip_spaces(path, &i);
    if (path[i] == '/')
        return TRUE;
    return FALSE;
}

BOOL   is_relative_path(char *path)
{
    int i;

    i = 0;
    skip_spaces(path, &i);
    if (path[i] && i < ((int)ft_strlen(path) - 1) && path[i] == '.' && path[i + 1] == '/')
        return TRUE;
    return FALSE;
}

