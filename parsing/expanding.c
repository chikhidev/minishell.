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
	return (ft_strdup(db, object->val));
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

int updated_line(t_db *db, char **line, char *variable_name, t_iterators *reminder)
{
    char *new_line;
    char *tmp;
    int reached;

    if (ft_strlen(variable_name) == 0)
        return reminder->i;
    tmp = ft_substr(db, *line, 0, reminder->i);
    if (!tmp)
        return error(db, NULL, "Malloc failed1");
    new_line = ft_strjoin(db, tmp, get_env(db, variable_name));
    if (!new_line)
        return error(db, NULL, "Malloc failed2");
    gc_free(db, tmp);
    reached = ft_strlen(new_line) - 1;
    tmp = ft_substr(db, *line, reminder->j, ft_strlen(*line));
    if (!tmp)
        return error(db, NULL, "Malloc failed3");

    gc_free(db, *line);
    *line = ft_strjoin(db, new_line, tmp);
    if (!*line)
        return error(db, NULL, "Malloc failed4");
    return reached;
}

int expand(t_db *db, char **line, t_quote **quotes)
{
    char    *env_var_name;
    int     i;
    t_iterators reminder;
    int len;

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
            if (!(*line)[++i]) return (SUCCESS);
            if (concat_env_name(db, line, &env_var_name, &i) == FAILURE)
                return (FAILURE);
            reminder.j = i;
            i = updated_line(db, line, env_var_name, &reminder);
            if (i == INVALID)
                return (FAILURE);
            len = ft_strlen(*line);
            if (i >= len)
                return (SUCCESS);
            gc_free(db, env_var_name);
            env_var_name = NULL;
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
