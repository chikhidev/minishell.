#include "main.h"
#include "parsing.h"

char    *get_environment_var(t_db *db, char   *var, char *env[])
{
    int i;
    char    *temp;


    i = 0;
    temp = ft_strjoin(db, var, "=");
    while (env[i])
    {
        if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
        {
            gc_free(db, temp);
            return env[i] + ft_strlen(var) + 1;
        }
        i++;
    }
    gc_free(db, temp);
    return NULL;
}

char *get_env(t_db *db, char *name)
{
    t_exp_list *object;

	if (!name)
        return (NULL);
    if (db->debug)
        printf("name -> %s\n", name);
    if (ft_strncmp(name, "?", ft_strlen(name)) == 0)
        return (ft_itoa(db, db->last_signal));
    if (are_all(name, ft_isdigit))
        return (ft_strdup(db, name + 1));
    object = get_exp_node(db->exp_list, name);

    if (object)
    {
        return ft_strdup(db, object->val);
    }

	return (ft_strdup(db, "''"));
}

int valid_char(char c, int  index)
{
    (void)index;
    if (isdigit(c))
        return (true);
    if (c == '?')
        return (true);
    if (c == '_' || ft_isalnum(c))
        return (true);
    return (false);
}

int concat_env_name(t_db *db, char **line, char **env_var_name, int *i)
{
    char    *tmp;

    tmp = NULL;
    while ((*line)[(*i)] && valid_char((*line)[*i], *i))
    {
        tmp = concat(db, *env_var_name, (*line)[(*i)]);
        if (!tmp)
            return (error(db, NULL, "Malloc failed7"));
        gc_free(db, *env_var_name);
        *env_var_name = tmp;
        (*i)++;
    }
    return (SUCCESS);
}

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

    tmp = ft_substr(db, *line, edges->j + 1, ft_strlen(*line));
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
        return reminder->i;
    
    tmp = get_env(db, variable_name);
    if (!tmp)
        return error(db, NULL, "Malloc failed4");

    if (update_index(db, line, tmp, reminder) == FAILURE)
        return INVALID;

    reached = reminder->i + ft_strlen(tmp) - 1;
    gc_free(db, tmp);
    return reached;
}

int expand(t_db *db, char **line, t_quote **quotes)
{
    char    *env_var_name;
    int     i;
    t_iterators reminder;
    int len;
    char *tmp;

    len = ft_strlen(*line);
    reminder.i = -1;
    reminder.j = -1;
    env_var_name = NULL;
    i = 0;
    while (i < len)
    {
        if ((*line)[i] == '$' && !inside_single_quote(*quotes, i))
        {
            reminder.i = i;

            if (!(*line)[++i])
            {
                return (SUCCESS);
            }

            if (
                    ((*line)[i] == '\'' || (*line)[i] == '\"')
                    && !is_inside_quotes_list(*quotes, i)
                )
            {
                reminder.i = i - 1; /* $ should be included int the trim, so dont include it */
                i++; /* skip the quote */

                // skip everything until the next quote and then continue
                while ((*line)[i] && (*line)[i] != '\'' && (*line)[i] != '\"')
                    i++;
                if (!(*line)[i])
                    return (SUCCESS);
                
                reminder.j = i + 1; /* include the quote */

                tmp = ft_substr(db, *line, reminder.i + 2, reminder.j - reminder.i - 3);
                if (!tmp)
                    return (FAILURE);
                
                i = update_index(db, line, tmp, &reminder);
                if (i == INVALID)
                    return (FAILURE);
            }
            else
            {
                if (concat_env_name(db, line, &env_var_name, &i) == FAILURE)
                    return (FAILURE);
                reminder.j = i - 1;

                i = updated_line(db, line, env_var_name, &reminder);
                if (i == INVALID)
                    return (FAILURE);
                
                gc_free(db, env_var_name);
                env_var_name = NULL;
            }

            len = ft_strlen(*line);
            if (i >= len)
                return (SUCCESS);

            reset_quotes(db, quotes);
            *quotes = NULL;

            if (!track_quotes(db, quotes, (*line))) return (FAILURE);
        }
        i++;
    }
    return (SUCCESS);
}


int expanded(t_db *db, char **args)
{
    t_quote *head;
    int64_t i;

    head = NULL;
    i = 0;
    while (args[i])
    {

        track_quotes(db, &head, args[i]);

        if (expand(db, &args[i], &head) == FAILURE)
            return FAILURE;

        args[i] = whithout_quotes(db, args[i]);

        if (args[i] == NULL)
            return error(db, NULL, "Malloc failed");

        i++;
    }

    return SUCCESS;
}
