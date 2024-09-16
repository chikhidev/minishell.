/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 01:54:14 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/16 21:56:18 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

char	**updated_result(char **result, char *save, bool cancel_split)
{
	char	**splitted;
	int		i;

	i = 0;
	if (len(without_quotes(this(), save, this()->q)) == 0)
	{
		add(this(), &result, ft_strdup(this(), ""));
		return (result);
	}
	if ((int)len(save) == 0)
		return (result);
	if (!cancel_split)
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

	if (len(save) == 0)
		return (result);
	q = NULL;
	if (track_quotes(db, &q, save) == FAILURE)
		return (NULL);
	cancel_split = (result && result[0] && ft_strcmp(result[0], "export") == 0)
		&& (ft_strsearch(save, '=') != NULL && (ft_strsearch(save, '=')
				+ 1) != NULL && *(ft_strsearch(save, '=') + 1) == '$' && (save
				&& save[0] != '$'));
	expand(&save, &q);
	if (len(save) == 0)
		return (result);
	db->q = q;
	return (updated_result(result, save, cancel_split));
}
