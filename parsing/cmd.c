/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:47:52 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/12 02:47:53 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

int	create_cmd_node(t_db *db, void **curr)
{
	*curr = gc_malloc(db, sizeof(t_cmd));
	if (!*curr)
		return (FAILURE);
	(((t_cmd *)*curr))->type = CMD_NODE;
	(((t_cmd *)*curr))->args = NULL;
	(((t_cmd *)*curr))->input_fd = STDIN_FILENO;
	(((t_cmd *)*curr))->output_fd = STDOUT_FILENO;
	return (SUCCESS);
}

int	count_op(char *line, int op)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (line[i])
	{
		if (is_op(line, &i) == op)
			count++;
		i++;
	}
	return (count);
}
