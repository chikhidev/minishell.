/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:07 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 23:55:04 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

/**
 * @param cut: the indexes of the start and the end to put the value
 */
int	update_index(t_db *db, char **line, char *var, t_str_cut *cut)
{
	char	*left_section;
	char	*right_section;
	char	*tmp;
	int		updated_index;

	right_section = NULL;
	tmp = NULL;
	left_section = ft_substr(db, *line, 0, cut->start_ignore);
	updated_index = 0;
	if (var != NULL)
	{
		cut->end_ignore += (int)len(var);
		tmp = get_env(db, var);
	}
	else if (cut->start_include != -1)
	{
		tmp = ft_substr(db, *line, cut->start_include, cut->end_include
				- cut->start_include + 1);
		updated_index = cut->end_include - 2;
	}
	tmp = ft_strjoin(db, left_section, tmp);
	right_section = ft_substr(db, *line, cut->end_ignore, (int)len(*line)
			- cut->end_ignore);
	*line = ft_strjoin(db, tmp, right_section);
	return (updated_index);
}

int	updated_line(t_db *db, char **line, char *var, t_str_cut *cut)
{
	if ((int)len(var) == 0)
		return (cut->end_ignore);
	return (update_index(db, line, var, cut));
}

bool	split_factor(char *value, char *line, int pos)
{
	(void)pos;
	(void)line;
	if ((int)len(value) == 0)
		return (false);
	return (true);
}
