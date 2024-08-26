#include "main.h"
#include "parsing.h"
#include "./index.h"

int expand(t_db *db, char **line, t_quote **quotes)
{
    char    *env_var_name;
    int     i;
    t_iterators rem;
    int len;

    len = ft_strlen(*line);
    rem.i = -1;
    rem.j = -1;
    env_var_name = NULL;
    i = 0;

    while (i < len)
    {
        if ((*line)[i] == '$' && !inside_single_quote(*quotes, i))
        {

            if (!(*line)[++i])
            {
                return (SUCCESS);
            }

            rem.i = i - 1;
            rem.j = i;

            if (is_quote_oppening(*quotes, i))
            {
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
                    return (FAILURE);
                
                gc_free(db, env_var_name);
                env_var_name = NULL;
            }

            len = ft_strlen(*line);
            if (i >= len)
                return (SUCCESS);

            reset_quotes(db, quotes);
            *quotes = NULL;

            if (quotes && !track_quotes(db, quotes, (*line)))
                return (FAILURE);
        }
        i++;
    }
    return (SUCCESS);
}
