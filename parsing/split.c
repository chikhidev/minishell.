/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:39 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 23:55:04 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"

void	init_vars_append(int *i, int *len, int *k, int flag)
{
	if (!flag)
	{
		*i = -1;
		*len = 0;
		*k = 0;
	}
	else
	{
		(*k)++;
		*len = 0;
	}
}

char	**split_line(t_db *db, char *line, t_op *op, t_tracker *tracker)
{
	int		i;
	int		len;
	int		k;
	char	**splitted;

	init_vars_append(&i, &len, &k, 0);
	splitted = gc_malloc(db, sizeof(char *) * op->n_childs);
	while (line[++i])
	{
		if (is_op(line, &i) == op->op_presentation
			&& !is_inside_quotes_list(tracker->quotes, i))
		{
			splitted[k] = gc_malloc(db, sizeof(char) * (len + 1));
			ft_strlcpy(splitted[k], line + i - len, len + 1);
			skip_op(&i, line);
			init_vars_append(&i, &len, &k, 1);
		}
		len++;
	}
	if (len > 0 && k < op->n_childs)
	{
		splitted[k] = gc_malloc(db, sizeof(char) * (len + 1));
		ft_strlcpy(splitted[k], line + i - len, len + 1);
	}
	return (splitted);
}

int	process_op(t_db *db, char *line, t_holder *holder)
{
	void	**curr;
	char	**splitted;
	int		i;

	curr = holder->current_node;
	if (create_op_node(db, holder->op, holder->current_node) == FAILURE)
	{
		return (FAILURE);
	}
	((t_op *)*curr)->n_childs = count_between_op(db, line, holder->op,
			holder->tracker);
	splitted = split_line(db, line, ((t_op *)*curr), holder->tracker);
	((t_op *)*curr)->childs = gc_malloc(db, sizeof(void *)
			* ((t_op *)*curr)->n_childs);
	ft_bzero(((t_op *)*curr)->childs, sizeof(void *)
		* ((t_op *)*curr)->n_childs);
	i = 0;
	while (i < ((t_op *)*curr)->n_childs)
	{
		if (smart_split(db, splitted[i], &((t_op *)*curr)->childs[i],
				*curr) == FAILURE)
			return (FAILURE);
		i++;
	}
	return (SUCCESS);
}

int	process_cmd(t_db *db, char *line, t_holder *holder)
{
	void	**curr;
	char	**splitted;
	t_quote	*quotes;

	curr = holder->current_node;
	quotes = NULL;
	if (create_cmd_node(db, curr) == FAILURE)
	{
		return (FAILURE);
	}
	if (track_quotes(db, &quotes, line) == FAILURE)
	{
		return (FAILURE);
	}
	splitted = tokenize(db, &quotes, line);
	if (db->error || !db->exec_line)
		return (error(db, NULL, NULL));
	((t_cmd *)*curr)->args = splitted;
	(((t_cmd *)*curr))->input_fd = db->input_fd;
	(((t_cmd *)*curr))->output_fd = db->output_fd;
	db->input_fd = STDIN_FILENO;
	db->output_fd = STDOUT_FILENO;
	return (SUCCESS);
}

int	smart_split(t_db *db, char *line, void **current_node, void *parent)
{
	t_holder	holder;

	if (db->error || !db->exec_line)
		return (FAILURE);
	if ((int)len(line) == 0 || all_whitespaces(line, 0, (int)len(line)))
		return (SUCCESS);
	ft_bzero(&holder, sizeof(holder));
	holder.tracker = gc_malloc(db, sizeof(t_tracker));
	holder.tracker->quotes = NULL;
	holder.parent = parent;
	track_quotes(db, &holder.tracker->quotes, line);
	holder.op = strongest_operator(line, holder.tracker->quotes);
	holder.current_node = current_node;
	if (holder.op != NOT_FOUND)
	{
		if (process_op(db, line, &holder) == FAILURE)
			return (FAILURE);
	}
	else if (process_cmd(db, line, &holder) == FAILURE)
		return (db->error != true);
	return (SUCCESS);
}
