/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string3.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:48:12 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:55:04 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

char	*sub(t_db *db, char *line, int i, int j)
{
	char	*res;

	res = gc_malloc(db, sizeof(char) * (j - i + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, line + i, j - i + 1);
	return (res);
}

/**
 * @details This function checks if the string is all whitespaces,
	pay attention that it frees the original string
 */
char	*whithout_quotes_free_db(t_db *db, char *line)
{
	int		i;
	char	*res;
	int		size;

	i = 1;
	size = (int)len(line);
	if (!((line[0] == SGL_QUOTE && line[size - 1] == SGL_QUOTE)
			|| (line[0] == DBL_QUOTE && line[size - 1] == DBL_QUOTE)))
		return (line);
	size -= 2;
	res = gc_malloc(db, sizeof(char) * (size + 1));
	if (!res)
		return (NULL);
	ft_strlcpy(res, line + i, size + 1);
	return (res);
}

short	inside_quot_v2(short single_opened, short double_opened)
{
	if (single_opened && !double_opened)
		return (true);
	else if (double_opened && !single_opened)
		return (true);
	else
		return (false);
}

bool	is_inside_quotes_line(char *line, int index)
{
	bool	in_single_quote;
	bool	in_double_quote;
	int		i;

	in_single_quote = false;
	in_double_quote = false;
	i = 0;
	while (i <= index)
	{
		if (line[i] == SGL_QUOTE)
			in_single_quote = !in_single_quote;
		else if (line[i] == DOBLQUOTE)
			in_double_quote = !in_double_quote;
		i++;
	}
	return (in_single_quote || in_double_quote);
}
