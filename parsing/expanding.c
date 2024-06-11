#include "../includes/main.h"
#include "../includes/parsing.h"

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
    if (!new_line) return error(db, "Failed to malloc new_line");
    ft_strlcpy(new_line, *original_line, a + 1); // from 0 to a0
    ft_strlcpy(new_line + a, env_variable, ft_strlen(env_variable) + 1); //from a + len dyal env
    ft_strlcpy(new_line + a + ft_strlen(env_variable), *original_line + a + b, c + 1); // from a + len +
    gc_free(db, *original_line);
    *original_line = new_line;
    *new_pos = a + ft_strlen(env_variable);
    free(env_variable);
    return (SUCCESS);
}

char *get_env(t_db *db, char *name)
{
    char *res;

	if (!name) return (NULL);
    printf("name -> %s\n", name);
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
    res = getenv(name);
    if (!res) return (ft_strdup(""));
	return (ft_strdup(res));
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
        if (!tmp) return error(db, "Concat failed");
        gc_free(db, *env_var_name);
        *env_var_name = tmp;
        (*i)++;
    }
    return (SUCCESS);
}

int expand(t_db *db, char **line)
{
    char    *env_var_name;
    int     i;

    env_var_name = NULL;
    i = 0;
    while ((*line)[i])
    {
        if ((*line)[i] == '$' && !inside_single_quote(db, i))
        {
            if (!(*line)[++i]) return (SUCCESS);
            if (concat_env_name(db, line, &env_var_name, &i) == FAILURE)
                return (FAILURE);
            if (update_env_in_line(db, line, get_env(db, env_var_name), &i) == FAILURE)
                return (FAILURE);
            if (!(*line)[i]) return (SUCCESS);
            gc_free(db, env_var_name);
            env_var_name = NULL;
            reset_quotes(db); // reset the quotes to stay updated
            if (!track_quotes(db, (*line))) return (FAILURE);
        }
        i++;
    }
    return (SUCCESS);
}
