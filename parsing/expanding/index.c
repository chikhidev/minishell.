#include "main.h"
#include "parsing.h"
#include "./index.h"

int expand(t_db *db, char **line, t_quote **quotes)
{
    char    *env_var_name;
    int     i;
    t_str_cut cut;
    int len;
    char *value;

    len = ft_strlen(*line);
    env_var_name = NULL;
    ft_bzero(&cut, sizeof(t_str_cut));
    i = 0;

    while (i < len)
    {
        // printf("i: %d\n", i);
        if ((*line)[i] == '$' && !(quotes && inside_single_quote(*quotes, i)))
        {
            if (!(*line)[++i])
                return (SUCCESS);

            cut.start_ignore = i - 1;
            cut.end_ignore = i;

            if ((quotes && is_quote(*quotes, i))
                && !is_inside_quotes_list(*quotes, i - 1))
            {
                printf("$\"\" found\n");
                i++;
                while ((*line)[i] && (*line)[i] != (*line)[cut.end_ignore])
                    i++;
                cut.start_include = cut.start_ignore + 2;
                cut.end_include = i - 1;
                cut.end_ignore = i + 1;
                i = update_index(db, line, NULL, &cut);
                delete_quotes_in_range(quotes, cut.start_ignore, cut.end_ignore);
            }
            else
            {
                printf("$ found\n");
                cut.start_include = -1;
                if (concat_env_name(line, &env_var_name, &i, *quotes) == FAILURE)
                    return (FAILURE);
                printf("env_var_name: %s\n", env_var_name);
                printf("i: %d\n", i);
                // export a="a b c"
                // echo "$a"
                i = updated_line(db, line, env_var_name, &cut);
            }

            if (env_var_name)
            {
                value = get_env(db, env_var_name);
                env_var_name = NULL;
                update_quotes(*quotes, cut.start_ignore, ft_strlen(env_var_name), ft_strlen(value));
            }
            
            if (i == INVALID)
                return (FAILURE);
            if (i >= len)
                return (SUCCESS);
            len = ft_strlen(*line);
        }
        i++;
    }
    return (SUCCESS);
}
