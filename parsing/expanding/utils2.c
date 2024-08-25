#include "main.h"
#include "parsing.h"
#include "./index.h"

/**
 * @param edges: the indexes of the start and the end to put the value
 */
int update_index(t_db *db, char **line, char *value, t_iterators *edges)
{
    char *tmp;
    char *tmp2;

    tmp = ft_substr(db, *line, 0, edges->i);
    if (!tmp)
        return error(db, NULL, "Malloc failed8");

    tmp2 = ft_strjoin(db, tmp, value);
    if (!tmp2)
        return error(db, NULL, "Malloc failed9");

    gc_free(db, tmp);

    if (value != NULL)
        edges->j++;
    tmp = ft_substr(db, *line, edges->j, ft_strlen(*line));
    if (!tmp)
        return error(db, NULL, "Malloc failed10");

    tmp2 = ft_strjoin(db, tmp2, tmp);
    if (!tmp2)
        return error(db, NULL, "Malloc failed11");

    gc_free(db, tmp);
    gc_free(db, *line);

    *line = tmp2;

    return SUCCESS;
}

int updated_line(t_db *db, char **line, char *variable_name, t_iterators *reminder)
{
    // char *new_line;
    char *tmp;
    int reached;

    if (ft_strlen(variable_name) == 0)
    {
        return reminder->i;
    }
    
    tmp = get_env(db, variable_name);
    if (!tmp)
        return error(db, NULL, "Malloc failed4");

    if (update_index(db, line, tmp, reminder) == FAILURE)
        return INVALID;

    reached = reminder->i + ft_strlen(tmp) - 1;
    gc_free(db, tmp);
    return reached;
}
