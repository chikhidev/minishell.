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
    tmp2 = ft_strjoin(db, tmp, value);
    if (value != NULL)
        edges->j++;
    tmp = ft_substr(db, *line, edges->j, ft_strlen(*line));
    tmp2 = ft_strjoin(db, tmp2, tmp);
    *line = tmp2;
    return SUCCESS;
}

int updated_line(t_db *db, char **line, char *variable_name, t_iterators *reminder)
{
    // char *new_line;
    char *tmp;

    if (ft_strlen(variable_name) == 0)
    {
        return reminder->i;
    }
    
    tmp = get_env(db, variable_name);
    update_index(db, line, tmp, reminder);

    if (ft_strlen(tmp) == 0)
        return reminder->i;
    return reminder->i + ft_strlen(tmp) - 1;
}
