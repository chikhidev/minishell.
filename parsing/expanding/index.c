// #include "./index.h"
#include "main.h"
#include "parsing.h"

int	expand(t_db *db, char **line, t_quote **quotes)
{
	char		*env_var_name;
	int			i;
	t_str_cut	cut;
	int			len;
	char		*value;
	t_quote		*q;

	db->split = false;
	q = NULL;
	len = ft_strlen(*line);
	env_var_name = NULL;
	ft_bzero(&cut, sizeof(t_str_cut));
	i = 0;
	while (i < len)
	{
		if ((*line)[i] == '$' && !(quotes && inside_single_quote(*quotes, i)))
		{
			printf("found $ in positioin %d, line: [%s]\n", i, *line);
			if (!(*line)[++i])
				return (SUCCESS);
			cut.start_ignore = i - 1;
			cut.end_ignore = i;
			if (quotes && is_quote_oppening(*quotes, i))
			{
				printf("1-found $ in positioin %d, line: [%s]\n", i, *line);
				q = quote_at(*quotes, i);
				/* this is impossible case OFC but we abviously need to check who knows*/
				if (!q)
					return error(db, NULL, "Something went wrong, was expecting a quote");
				cut.start_include = q->start;
				cut.end_include = q->end;
				cut.end_ignore = cut.end_include + 1;
				update_index(db, line, NULL, &cut);


				update_quotes(*quotes, cut.start_ignore,
					cut.end_ignore - cut.start_ignore,
					cut.end_ignore - cut.start_ignore - 1);
				
				/* return to start of the quote:: */
				q = quotes_of_range(*quotes, cut.start_include);
				/* this is impossible case OFC but we abviously need to check who knows*/
				if (!q)
					return error(db, NULL, "Something went wrong, was expecting a quote");
				i = q->start;
				printf(GREEN"i->%d, line-> [%s]\n"RESET, i, (*line));
			}
			else
			{
				printf("2-found $ in positioin %d, line: [%s]\n", i, *line);
				db->split = !is_inside_quotes_list(*quotes, i);
				cut.start_include = -1;
				concat_env_name(line, &env_var_name, &i,
						*quotes);
				printf(ORANGE"env_var_name->[%s]\n"RESET, env_var_name);

				if (env_var_name)
				{
					i = updated_line(db, line, env_var_name, &cut);

					value = get_env(db, env_var_name);
					update_quotes(*quotes, cut.start_ignore,
						ft_strlen(env_var_name) + 1, ft_strlen(value));

					db->split *= split_factor(value, *line, cut.start_ignore);

					env_var_name = NULL;

					if ((*line)[i] == '$' && !(quotes && inside_single_quote(*quotes, i)))
						i -= 1;
				}
			}
			len = ft_strlen(*line);
			if (i >= len)
				return (SUCCESS);
		}
		i++;
	}
	return (SUCCESS);
}
