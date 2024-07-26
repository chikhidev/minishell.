#include "main.h"
#include "parsing.h"

void    calc_line(char *original_line, int *a, int *b, int *c)
{
    while (original_line[*a + *b + *c] && original_line[*a + *b + *c] != '$')
        (*a)++;
    *b += (original_line[*a + *b + *c] == '$');
    while (original_line[*a + *b + *c] && valid_char(original_line[*a + *b + *c], *a + *b + *c))
        (*b)++;
    while (original_line[*a + *b + *c])
        (*c)++;

}

/**
 * @details This function's job is to update the line with the env variables
*/
int    update_env_in_line(t_db *db, char **original_line, char *env_variable, int *new_pos)
{
    char    *new_line;
    int     a;
    int     b;
    int     c;

    if (!env_variable) return (SUCCESS);
    a = 0;
    b = 0;
    c = 0;
    calc_line(*original_line, &a, &b, &c);
    new_line = gc_malloc(db, sizeof(char) * (a + ft_strlen(env_variable) + c + 1));
    CATCH_MALLOC(new_line);
    // if (!new_line) return error(db, NULL, "Failed to malloc new_line");
    ft_strlcpy(new_line, *original_line, a + 1); // from 0 to a0
    ft_strlcpy(new_line + a, env_variable, ft_strlen(env_variable) + 1); //from a + len dyal env
    ft_strlcpy(new_line + a + ft_strlen(env_variable), *original_line + a + b, c + 1); // from a + len +
    gc_free(db, *original_line);
    *original_line = new_line;
    *new_pos = a + ft_strlen(env_variable);
    free(env_variable);
    return (SUCCESS);
}

char    *get_environment_var(char   *var, char *env[])
{
    int i;
    char    *temp;
    

    i = 0;
    temp = ft_strjoin(var, "=");
    while (env[i])
    {
        if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
        {
            free(temp);
            return env[i] + ft_strlen(var) + 1;
        }
        i++;
    }
    free(temp);
    return NULL;
}

char *get_env(t_db *db, char *name)
{
    t_exp_list *object;

	if (!name) return (NULL);
    if (db->debug) printf("name -> %s\n", name);
    if (ft_strncmp(name, "?", ft_strlen(name)) == 0)
    {
        return (ft_itoa(db->last_signal));
    }
    if (are_all(name, ft_isdigit))
    {
        return (ft_strdup(name + 1));
    }
    // if (ft_strncmp(name, "_", ft_strlen(name)) == 0)
    // {
    //     return (ft_strdup(db->last_cmd));
    // }
    // res = getenv(name);
    object = get_exp_node(db->exp_list, name);
    CATCH_ONNULL(
        object,
        ft_strdup("")
    )
    printf("value bringed: %s\n", object->val);
	return (ft_strdup(object->val));
}

int valid_char(char c, int  index)
{
    (void)index;
    if (isdigit(c))
        return (TRUE);
    if (c == '?')
        return (TRUE);
    if (c == '_' || ft_isalnum(c))
        return (TRUE);
    return (FALSE);
}

int concat_env_name(t_db *db, char **line, char **env_var_name, int *i)
{
    char    *tmp;

    while ((*line)[(*i)] && valid_char((*line)[*i], *i))
    {
        tmp = concat(db, *env_var_name, (*line)[(*i)]);
        CATCH_MALLOC(tmp);
        gc_free(db, *env_var_name);
        *env_var_name = tmp;
        (*i)++;
    }
    return (SUCCESS);
}

int expand(t_db *db, char **line, t_quote *quotes)
{
    char    *env_var_name;
    int     i;

    env_var_name = NULL;
    i = 0;
    while ((*line)[i])
    {
        if ((*line)[i] == '$' && !inside_single_quote(quotes, i))
        {
            if (!(*line)[++i]) return (SUCCESS);
            if (concat_env_name(db, line, &env_var_name, &i) == FAILURE)
                return (FAILURE);
            if (update_env_in_line(db, line, get_env(db, env_var_name), &i) == FAILURE)
                return (FAILURE);
            if (!(*line)[i]) return (SUCCESS);
            gc_free(db, env_var_name);
            env_var_name = NULL;
            reset_quotes(db, &quotes);
            if (!track_quotes(db, &quotes, (*line))) return (FAILURE);
        }
        i++;
    }
    return (SUCCESS);
}
