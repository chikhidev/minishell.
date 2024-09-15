/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:48:11 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:58:11 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

int	strongest_operator(char *line, t_quote *q)
{
	int	i;
	int	op;

	i = 0;
	while (line[i])
	{
		op = is_op(line, &i);
		if (op != INVALID && !is_inside_quotes_list(q, i))
		{
			return (op);
		}
		i++;
	}
	return (NOT_FOUND);
}

// this one is not secure
int	count_between_op(t_db *db, char *line, int op, t_tracker *tracker)
{
	int	i;
	int	counter;
	int	reminder;

	(void)db;
	i = 0;
	reminder = 0;
	counter = 0;
	skip_spaces(line, &i);
	while (line[i])
	{
		if (is_op(line, &i) == op && !is_inside_quotes_list(tracker->quotes, i))
		{
			counter++;
			reminder = i;
		}
		i++;
	}
	if (reminder < i && !all_whitespaces(line, reminder, i)
		&& !is_inside_quotes_list(tracker->quotes, i))
		counter++;
	return (counter);
}

int	count_array_len(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		i++;
	return (i);
}

bool	same_s(char *s1, char *s2)
{
	return (ft_strcmp(s1, s2) == 0);
}
