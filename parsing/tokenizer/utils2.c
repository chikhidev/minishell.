/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 01:54:14 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 23:55:04 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

char	**updated_result(char **result, char *save, bool empty_quotes,
		bool cancel_split)
{
	char	**splitted;
	int		i;

	i = 0;
	if (empty_quotes)
	{
		add(this(), &result, ft_strdup(this(), ""));
		return (result);
	}
	if ((int)len(save) == 0)
		return (result);
	if (this()->split && !cancel_split)
	{
		splitted = ft_split(this(), save, " \t\n\r\v\f");
		i = 0;
		while (splitted && splitted[i])
		{
			add(this(), &result, splitted[i]);
			i++;
		}
		return (result);
	}
	add(this(), &result, save);
	return (result);
}

char	**append_word(t_db *db, char **result, char *save)
{
	t_quote	*q;
	bool	cancel_split;
	bool	empty_quotes;

	if ((int)len(save) == 0)
	{
		return (result);
	}
	q = NULL;
	if (track_quotes(db, &q, save) == FAILURE)
		return (NULL);
	cancel_split = (result && result[0] && ft_strcmp(result[0], "export") == 0)
		&& (ft_strsearch(save, '=') != NULL && (ft_strsearch(save, '=')
				+ 1) != NULL && *(ft_strsearch(save, '=') + 1) == '$'
			&& *(save) != '$');
	expand(db, &save, &q);
	empty_quotes = (q && q->start == 0 && q->end == ((int)(int)len(save) - 1));
	save = without_quotes(db, save, q);
	empty_quotes = empty_quotes && ((int)len(save) == 0);
	return (updated_result(result, save, empty_quotes, cancel_split));
}
