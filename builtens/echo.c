/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:27:29 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:59:10 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"
#include "string.h"

bool	is_new_line_op(char *arg)
{
	int	i;

	i = 1;
	if (!arg[0] || arg[0] != '-')
		return (false);
	if (!arg[1])
		return (false);
	while (arg[i])
	{
		if (arg[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	get_start_idx(char **args, bool *new_line)
{
	int	i;
	int	skip;

	i = 1;
	*new_line = true;
	skip = 1;
	while (args[i])
	{
		if (is_new_line_op(args[i]))
		{
			skip++;
			*new_line = false;
		}
		if (!is_new_line_op(args[i]))
			return (skip);
		i++;
	}
	return (skip);
}

int	echo_(t_db *db, char *args[])
{
	int		i;
	bool	new_line;
	int		n_args;

	(void)db;
	n_args = count_array_len(args);
	i = get_start_idx(args, &new_line);
	while (i < n_args)
	{
		ft_write(db, 1, args[i], (int)len(args[i]));
		i++;
		if (i != n_args)
			ft_write(db, 1, " ", 1);
	}
	if (new_line)
		ft_write(db, 1, "\n", 1);
	return (0);
}
