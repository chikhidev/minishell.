#include "main.h"
#include "parsing.h"
#include "./index.h"

int expand(t_db *db, char **line, t_quote **quotes)
{
    char    *env_var_name;
    int     i;
    t_iterators rem;
    int len;
    char *value;

    len = ft_strlen(*line);
    rem.i = -1;
    rem.j = -1;
    env_var_name = NULL;
    i = 0;

    while (i < len)
    {
        if ((*line)[i] == '$' && !(quotes && inside_single_quote(*quotes, i)))
        {
            if (!(*line)[++i])
                return (SUCCESS);

            rem.i = i - 1;
            rem.j = i;

            if ((quotes && is_quote_oppening(*quotes, i)))
            {
                if (update_index(db, line, NULL, &rem) == FAILURE)
                    return FAILURE;

                i++;
                while ((*line)[i] && (*line)[i] != (*line)[rem.j])
                    i++;

                rem.j = i;
                rem.i = rem.i - 1;

                if (update_index(db, line, NULL, &rem) == FAILURE)
                    return FAILURE;
            }
            else
            {
                if (concat_env_name(db, line, &env_var_name, &i) == FAILURE)
                    return (FAILURE);
                
                rem.j = i - 1;

                
                i = updated_line(db, line, env_var_name, &rem);
                
                if (i == INVALID)
                {
                    printf("invalid\n");
                    return (FAILURE);
                }
                
            }

            len = ft_strlen(*line);
            if (i >= len)
                return (SUCCESS);

            value = get_env(db, env_var_name);
            update_quotes(*quotes, rem.i, ft_strlen(env_var_name), ft_strlen(value));
            gc_free(db, env_var_name);
            env_var_name = NULL;
        }
        i++;
    }
    return (SUCCESS);
}
