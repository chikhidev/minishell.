#include "main.h"
#include "parsing.h"
#include "./index.h"

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
    if (ft_strncmp(name, "?", ft_strlen(name)) == 0)
        return (ft_itoa(db, db->last_status));
    if (are_all(name, ft_isdigit))
        return (ft_strdup(db, name + 1));
    object = get_exp_node(db->exp_list, name);

    if (object)
    {
        return ft_strdup(db, object->val);
    }

	return (ft_strdup(db, ""));
}

int valid_char(char c, int  index)
{
    (void)index;
    if (isdigit(c) || ft_isalnum(c))
        return (true);
    if (c == '?' || c == '_')
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
