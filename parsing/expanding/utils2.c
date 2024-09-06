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
    char *tmp3;

    if (!value && cut->start_ignore == -1)
        return SUCCESS;

    tmp = ft_substr(db, *line, 0, cut->start_ignore);
    
    tmp2 = NULL;
    if (value != NULL)
    {
        tmp2 = ft_strjoin(db, tmp, value);
        cut->end_ignore++;
    }
    else
    {
        tmp3 = ft_substr(db, *line, cut->start_include, cut->end_include - cut->start_include + 1);
        // printf("tmp3: %s\n", tmp3);
        tmp2 = ft_strjoin(db, tmp, tmp3);
        // printf("tmp2: %s\n", tmp2);
    }
        
    tmp = ft_substr(db, *line, cut->end_ignore, ft_strlen(*line) - cut->end_ignore + 1);
    // printf("tmp: %s\n", tmp);
    tmp2 = ft_strjoin(db, tmp2, tmp);
    *line = tmp2;
    return SUCCESS;
}

int updated_line(t_db *db, char **line, char *variable_name, t_str_cut *cut)
{
    char *tmp;

    if (ft_strlen(variable_name) == 0)
    {
        // printf("variable_name is empty\n");
        return cut->end_ignore;
    }
    
    tmp = get_env(db, variable_name);
    update_index(db, line, tmp, cut);

    if (ft_strlen(tmp) == 0)
        return cut->start_ignore;
    return cut->start_ignore + ft_strlen(tmp) - 1;
}
