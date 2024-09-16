/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:30 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/16 17:24:13 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"

int	parser(t_db *db, char *line)
{
	t_quote	*quotes;
	int		i;

	quotes = NULL;
	i = 0;
	if ((int)len(line) == 0)
		return (SUCCESS);
	skip_spaces(line, &i);
	if (line[i] == '\0')
		return (SUCCESS);
	if (track_quotes(db, &quotes, line) == FAILURE)
		return (FAILURE);
	if (syntax_checker(db, line, quotes) == FAILURE)
		return (db->last_status = 2, FAILURE);
	if (smart_split(db, line, &db->root_node, NULL) == FAILURE)
	{
		return (FAILURE);
	}
	return (SUCCESS);
}
