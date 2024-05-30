#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function's job is to update the line with the env variables
*/
// int    update_env_in_line(t_db *db, char **original_line, char *env_variable)
// {
//     char    *new_line;
//     int     a;
//     int     b;
//     int     c;

//     if (!env_variable) return (SUCCESS);
//     a = 0;
//     b = 0;
//     c = 0;
//     while ((*original_line)[a + b + c] && (*original_line)[a + b + c] != '$')
//         a++;
//     b += ((*original_line)[a + b + c] == '$');
//     while ((*original_line)[a + b + c] && valid_char((*original_line)[a + b + c], a + b + c))
//         b++;
//     while ((*original_line)[a + b + c])
//         c++;
//     new_line = gc_malloc(db, sizeof(char) * (a + ft_strlen(env_variable) + c + 1));
//     // printf("new linelen -> %zu\n", (a + ft_strlen(env_variable) + c + 1));
//     if (!new_line) return error(db, "Failed to malloc new_line");
//     ft_strlcpy(new_line, *original_line, a + 1); // from 0 to a0
//     ft_strlcpy(new_line + a, env_variable, ft_strlen(env_variable) + 1); //from a + len dyal env
//     ft_strlcpy(new_line + a + ft_strlen(env_variable), *original_line + a + b, c + 1); // from a + len +
//     gc_free(db, *original_line);
//     *original_line = new_line;
//     return (SUCCESS);
// }

char *get_env(t_db *db, char *name)
{
	if (!name) return (NULL);
    printf("name -> %s\n", name);
    if (ft_strncmp(name, "?", ft_strlen(name)) == 0)
    {
        return (ft_itoa(db->last_signal));
    }
	return (ft_strdup(getenv(name)));
}

// int concat_env_name(t_db *db, char **line, char **env_var_name, int *i)
// {
//     char    *tmp;

//     while ((*line)[(*i)] && valid_char((*line)[*i], *i))
//     {
//         tmp = concat(db, *env_var_name, (*line)[(*i)]);
//         if (!tmp) return error(db, "Concat failed");
//         gc_free(db, *env_var_name);
//         *env_var_name = tmp;
//         (*i)++;
//     }
//     return (SUCCESS);
// }

int count_dollars(t_db *db, char *line)
{
    int i;
    int count;

    i = 0;
    count = 0;
    while (line[i])
    {
        if (line[i] == '$' && !inside_single_quote(db, i))
            count++;
        i++;
    }
    return (count);
}

int is_env_var(t_db *db, char *line, int i)
{
    int digits_found;

    digits_found = 0;

    if (!line[i])
        return (FALSE);
    if (line[i] == '?' && i == 0)
        return (TRUE);
    while (ft_isalnum(line[i]) || line[i] == '_')
    {
        if (ft_isdigit(line[i]))
            digits_found = 1;
        else if (ft_isalpha(line[i]) && digits_found)
            return error(db, "Invalid env variable name");
        else if (!ft_isalnum(line[i]) && line[i] != '_')
            return (FALSE);
        i++;
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
        if ((*line)[i] == '$' && !inside_single_quote(db, i)
            && is_env_var(db, *line, ++i))
        {
            gc_free(db, env_var_name);
            env_var_name = NULL;
            while (ft_isalnum((*line)[i]) || (*line)[i] == '_')
            {
                env_var_name = concat(db, env_var_name, (*line)[i]);
                i++;
            }
            printf("env-> %s\n", env_var_name);
        }
        if (!(*line)[i]) return (SUCCESS);
        i++;
    }


    // char    *env_var_name;
    // int     i;
    // int     len;
    // env_var_name = NULL;
    // i = 0;
    // len = ft_strlen(*line);
    // while (i < len && (*line)[i])
    // {
    //     len = ft_strlen(*line);
    //     if ((*line)[i] == '$' && !inside_single_quote(db, i))
    //     {
    //         if (!(*line)[++i]) return (SUCCESS);
    //         if (concat_env_name(db, line, &env_var_name, &i) == FAILURE)
    //             return (FAILURE);
    //         if (update_env_in_line(db, line, get_env(db, env_var_name)) == FAILURE)
    //             return (FAILURE);
    //         gc_free(db, env_var_name);
    //         env_var_name = NULL;
    //         reset_quotes(db); // reset the quotes to stay updated
    //         if (!track_quotes(db, (*line))) return (FAILURE);
    //     }
    //     i++;
    // }
    return (SUCCESS);
}
