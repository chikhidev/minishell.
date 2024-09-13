/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string8.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 17:03:21 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/13 17:05:14 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"
#include "string.h"

char	*without_quotes(t_db *db, char *str, t_quote *q)
{
	char	*res;
	int		i;

	if (!q)
		return (str);
	res = NULL;
	i = 0;
	while (str[i])
	{
		if (!is_quote(q, i))
		{
			res = concat(db, res, str[i]);
		}
		i++;
	}
	return (res);
}

bool	contains(char *str, char *sub)
{
	int	i;
	int	j;

	i = 0;
	if (!str)
		return (false);
	if (!sub)
		return (true);
	while (str[i])
	{
		j = 0;
		while (str[i + j] == sub[j] && str[i + j] && sub[j])
			j++;
		if (!sub[j])
			return (true);
		i++;
	}
	return (false);
}
