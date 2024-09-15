/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   op2.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:28 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 23:47:40 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

int	is_op(char *line, int *i)
{
	if (line[*i] == '|')
		return (PIPE);
	return (INVALID);
}

int	create_op_node(t_db *db, int op, void **curr)
{
	*curr = gc_malloc(db, sizeof(t_op));
	((t_op *)*curr)->type = OP_NODE;
	((t_op *)*curr)->op_presentation = op;
	((t_op *)*curr)->n_childs = 0;
	((t_op *)*curr)->input_fd = STDIN_FILENO;
	((t_op *)*curr)->output_fd = STDOUT_FILENO;
	return (SUCCESS);
}
