#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function's job is to update the line with the env variables
*/
int    update_env_in_line(t_db *db, char **original_line, char *env_variable)
{
    char    *new_line;
    int     a;
    int     b;
    int     c;

    if (!env_variable) return (SUCCESS);
    a = 0;
    b = 0;
    c = 0;
    while ((*original_line)[a + b + c] && (*original_line)[a + b + c] != '$')
        a++;
    b += ((*original_line)[a + b + c] == '$');
    while ((*original_line)[a + b + c] && valid_char((*original_line)[a + b + c], a + b + c))
        b++;
    while ((*original_line)[a + b + c])
        c++;
    new_line = gc_malloc(db, sizeof(char) * (a + ft_strlen(env_variable) + c + 1));
    // printf("new linelen -> %zu\n", (a + ft_strlen(env_variable) + c + 1));
    if (!new_line) return error(db, "Failed to malloc new_line");
    ft_strlcpy(new_line, *original_line, a + 1); // from 0 to a0
    ft_strlcpy(new_line + a, env_variable, ft_strlen(env_variable) + 1); //from a + len dyal env
    ft_strlcpy(new_line + a + ft_strlen(env_variable), *original_line + a + b, c + 1); // from a + len +
    gc_free(db, *original_line);
    *original_line = new_line;
    return (SUCCESS);
}

char *get_env(t_db *db, char *name)
{
	int i;

	i = 0;
	while (db->env[i])
    {
		if (ft_strncmp(name, db->env[i], ft_strlen(name)) == 0 && db->env[i][ft_strlen(name)] == '=')
			return (db->env[i] + ft_strlen(name) + 1);
		i++;
    }
	return (NULL);
}

int valid_char(char c, int  index)
{
    if (isdigit(c) && index == 0)
        return (0);
    if (c == '_' || ft_isalnum(c))
        return (1);
    return (0);
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
    int     len;
    env_var_name = NULL;
    i = 0;
    len = ft_strlen(*line);
    while (i < len && (*line)[i])
    {
        len = ft_strlen(*line);
        if ((*line)[i] == '$' && !inside_single_quote(db, i))
        {
            if (!(*line)[++i]) return (SUCCESS);
            if (concat_env_name(db, line, &env_var_name, &i) == FAILURE)
                return (FAILURE);
            if (update_env_in_line(db, line, get_env(db, env_var_name)) == FAILURE)
                return (FAILURE);
            gc_free(db, env_var_name);
            env_var_name = NULL;
            reset_quotes(db); // reset the quotes to stay updated
            if (!track_quotes(db, (*line))) return (FAILURE);
        }
        i++;
    }
    return (SUCCESS);
}
