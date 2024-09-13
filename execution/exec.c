/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:48:11 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/13 10:41:22 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "parsing.h"

char	*get_path(t_db *db, char **args)
{
	char	*path;

	path = args[0];
	if (ft_strcmp(path, ".") == 0 || ft_strcmp(path, "..") == 0)
		return (ft_strdup(db, path));
	if (is_str_empty(db, args[0]))
		ft_exit(db, 127, 3, ft_strjoin(db, args[0], ": command not found"));
	if (is_relative_path(path) || is_absolute_path(path))
	{
		if (access(path, F_OK) != 0)
		{
			(perror(path), fd_void(db), ec_void(db));
			exit(error(db, NULL, NULL) + 127);
		}
		else if (access(path, X_OK) != 0)
		{
			(perror(path), fd_void(db), ec_void(db));
			exit(error(db, NULL, NULL) + 126);
		}
	}
	else
	{
		path = cmd_path(db, args[0]);
		if (db->error)
			(fd_void(db), exit(db->last_status));
		if (!path)
			ft_exit(db, 127, 3, ft_strjoin(db, args[0], ": command not found"));
	}
	return (path);
}

void	handle_pipe_op(t_db *db, void *node)
{
	int	i;
	int	**pipes;

	i = 0;
	pipes = prepare_pipes(db, ((t_op *)node)->n_childs - 1);
	while (i < ((t_op *)node)->n_childs)
	{
		handle_cmd_node(db, ((t_op *)node)->childs[i], pipes, i);
		i++;
	}
	close_all_pipes(db, pipes);
	waiter(db);
	return ;
}

void	exec(t_db *db, void *node)
{
	if (!node)
		return ;
	if (((t_cmd *)node)->type == CMD_NODE)
		handle_cmd_node(db, node, NULL, -1);
	else if (((t_op *)node)->op_presentation == PIPE)
		handle_pipe_op(db, node);
}
