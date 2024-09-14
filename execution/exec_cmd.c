/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:48:09 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 21:45:18 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "parsing.h"

void	exec_cmd(t_db *db, void *node, int **pipes, int index)
{
	char	**env_arr;
	char	*path;

	if (((t_cmd *)node)->input_fd == INVALID)
		ft_exit(db, 1, 3, NULL);
	if (((t_cmd *)node)->output_fd == INVALID)
		ft_exit(db, 1, 3, NULL);
	path = get_path(db, ((t_cmd *)node)->args);
	env_arr = env_list_to_env_arr(db);
	default_signals_behav();
	dup_pipes(db, pipes, index);
	close_all_pipes(db, pipes);
	dup_cmd_io(db, node);
	handle_is_dir(db, ((t_cmd *)node)->args[0]);
	execve(path, ((t_cmd *)node)->args, env_arr);
	ft_exit(db, 127, 3, ft_strjoin(db, path, ": failed"));
}

int	handle_single_builtin(t_db *db, void *node, int index)
{
	int	in;
	int	out;

	if (((t_cmd *)node)->input_fd == INVALID)
		return (db->last_status = 1, 1);
	if (((t_cmd *)node)->output_fd == INVALID)
		return (db->last_status = 1, 1);
	in = ft_dup(db, STDIN_FILENO);
	out = ft_dup(db, STDOUT_FILENO);
	dup_cmd_io(db, node);
	db->last_status = run_builtin(db, node, index);
	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	return (db->last_status);
}

int	handle_builtin(t_db *db, void *node, int **pipes, int index)
{
	int	id;

	if (index == -1)
		return (handle_single_builtin(db, node, index));
	id = fork();
	if (id == CHILD)
	{
		signal(SIGINT, SIG_DFL);
		if (((t_cmd *)node)->input_fd == INVALID)
			exit(1);
		if (((t_cmd *)node)->output_fd == INVALID)
			exit(1);
		dup_pipes(db, pipes, index);
		dup_cmd_io(db, node);
		close_all_pipes(db, pipes);
		db->last_status = run_builtin(db, node, index);
		ft_exit(db, db->last_status, 3, NULL);
	}
	else
		pid_add(db, id);
	return (db->last_status);
}

void	handle_cmd_node(t_db *db, void *node, int **pipes, int index)
{
	int	id;
	int	status;

	if (!node || !((t_cmd *)node)->args || !((t_cmd *)node)->args[0])
		return ;
	if (index == -1)
		handle_underscore(db, node);
	status = 0;
	if (is_built_in(node))
		handle_builtin(db, node, pipes, index);
	else
	{
		id = fork();
		if (id == CHILD){
			exec_cmd(db, node, pipes, index);
		}
		else
		{
			handle_parent_signals();
			if (index == -1)
				(waitpid(id, &status, 0), catch_feedback(db, status));
			else
				pid_add(db, id);
		}
	}
}
