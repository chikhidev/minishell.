/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:47:57 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 01:24:39 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

int	gettext(char **line, t_str_cut *cut, t_quote **quotes, int *i)
{
	t_db	*db;
	t_quote	*q;

	db = this();
	q = quote_at(*quotes, *i);
	cut->start_include = q->start;
	cut->end_include = q->end;
	cut->end_ignore = cut->end_include + 1;
	update_index(db, line, NULL, cut);
	update_quotes(*quotes, cut->start_ignore, cut->end_ignore
		- cut->start_ignore, cut->end_ignore - cut->start_ignore - 1);
	q = quotes_of_range(*quotes, cut->start_include);
	*i = q->start;
	return (SUCCESS);
}

int	update_using_var(char **line, t_str_cut *cut, t_quote **quotes,
		char *env_var_name)
{
	t_db	*db;
	char	*value;
	int		idx;

	db = this();
	idx = updated_line(db, line, env_var_name, cut);
	value = get_env(db, env_var_name);
	if (quotes)
		update_quotes(*quotes, cut->start_ignore, ft_strlen(env_var_name) + 1,
			ft_strlen(value));
	db->split = db->split && split_factor(value, *line, cut->start_ignore);
	env_var_name = NULL;
	if ((*line)[idx] == '$' && !(quotes && inside_single_quote(*quotes, idx)))
		idx -= 1;
	return (idx);
}

int	normal(char **line, t_str_cut *cut, t_quote **quotes, int *i)
{
	t_db	*db;
	char	*env_var_name;

	db = this();
	env_var_name = NULL;
	if (quotes)
		db->split = !is_inside_quotes_list(*quotes, *i);
	cut->start_include = -1;
	if (quotes)
		concat_env_name(line, &env_var_name, i, *quotes);
	else
		concat_env_name(line, &env_var_name, i, NULL);
	if (!env_var_name)
		return (SUCCESS);
	*i = update_using_var(line, cut, quotes, env_var_name);
	return (SUCCESS);
}

int	switch_(char **line, t_str_cut *cut, t_quote **quotes, int *i)
{
	if (quotes && is_quote_oppening(*quotes, *i))
	{
		if (gettext(line, cut, quotes, i) == FAILURE)
			return (FAILURE);
	}
	else
	{
		if (normal(line, cut, quotes, i) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

int	expand(t_db *db, char **line, t_quote **quotes)
{
	int			i;
	t_str_cut	cut;
	int			len;

	db->split = false;
	len = ft_strlen(*line);
	ft_bzero(&cut, sizeof(t_str_cut));
	i = 0;
	while (i < len)
	{
		if ((*line)[i] == '$' && !(quotes && inside_single_quote(*quotes, i)))
		{
			if (!(*line)[++i])
				return (SUCCESS);
			cut.start_ignore = i - 1;
			cut.end_ignore = i;
			if (switch_(line, &cut, quotes, &i) == FAILURE)
				return (FAILURE);
			len = ft_strlen(*line);
		}
		i++;
	}
	return (SUCCESS);
}
