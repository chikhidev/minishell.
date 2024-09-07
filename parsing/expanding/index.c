#include "./index.h"
#include "main.h"
#include "parsing.h"

int	expand(t_db *db, char **line, t_quote **quotes)
{
	char		*env_var_name;
	int			i;
	t_str_cut	cut;
	int			len;
	char		*value;

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
				cut.start_include = quote_at(*quotes, i)->start + 1;
				cut.end_include = quote_at(*quotes, i)->end - 1;
				cut.end_ignore = cut.end_include + 2;
				i = update_index(db, line, NULL, &cut);
				delete_quotes_in_range(quotes, cut.start_ignore,
					cut.end_ignore);
			}
			else
			{
				db->split = !is_inside_quotes_list(*quotes, i);
				cut.start_include = -1;
				if (concat_env_name(line, &env_var_name, &i,
						*quotes) == FAILURE)
					return (FAILURE);
				i = updated_line(db, line, env_var_name, &cut);
				db->split *= (ft_strlen(env_var_name) > 0);
			}
			if (env_var_name)
			{
				value = get_env(db, env_var_name);
				update_quotes(*quotes, cut.start_ignore,
					ft_strlen(env_var_name), ft_strlen(value));
				env_var_name = NULL;
			}
			if (i == INVALID)
				return (FAILURE);
			len = ft_strlen(*line);
			if (i >= len)
				return (SUCCESS);
		}
		i++;
	}
	return (SUCCESS);
}
