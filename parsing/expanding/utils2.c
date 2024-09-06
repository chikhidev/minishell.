#include "main.h"
#include "parsing.h"
#include "./index.h"

/**
 * @param cut: the indexes of the start and the end to put the value
 */
int update_index(t_db *db, char **line, char *value, t_str_cut *cut)
{
    char *tmp;
    char *tmp2;

    tmp = ft_substr(db, *line, 0, cut->start_ignore);
    
    if (value != NULL)
    {
        tmp = ft_strjoin(db, tmp, value);
        cut->end_ignore = cut->start_ignore + ft_strlen(value) - 1;
    }
    else if (cut->start_include != -1)
    {
        tmp2 = ft_substr(db, *line, cut->start_include, cut->end_include - cut->start_include + 1);
        tmp = ft_strjoin(db, tmp, tmp2);
    }

    tmp2 = ft_substr(db, *line, cut->end_ignore, ft_strlen(*line) - cut->end_ignore);
    *line = ft_strjoin(db, tmp, tmp2);
    return SUCCESS;
}

int updated_line(t_db *db, char **line, char *val, t_str_cut *cut)
{
    char *tmp;

    if (ft_strlen(val) == 0)
        return cut->end_ignore;
    tmp = get_env(db, val);
    update_index(db, line, tmp, cut);
    if (ft_strlen(tmp) == 0)
        return cut->start_ignore;

    return cut->start_ignore + ft_strlen(tmp) - 1;
}
