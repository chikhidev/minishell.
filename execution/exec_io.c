/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_io.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:48:10 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 20:55:30 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "parsing.h"

int	**prepare_pipes(t_db *db, int n_pipes)
{
	int	**pipes;
	int	i;

	i = 0;
	pipes = gc_malloc(db, (sizeof(int *)) * (n_pipes + 1));
	while (i < n_pipes)
	{
		pipes[i] = gc_malloc(db, sizeof(int) * 2);
		ft_pipe(db, pipes[i]);
		i++;
	}
	pipes[n_pipes] = NULL;
	return (pipes);
}

int	dup_pipes(t_db *db, int **pipes, int index)
{
	int	n_pipes;

	n_pipes = get_pipes_count(pipes);
	if (index == -1)
		return (SUCCESS);
	if (index == 0)
		ft_dup2(db, pipes[0][1], STDOUT_FILENO);
	else if (index == n_pipes)
	{
		ft_dup2(db, pipes[n_pipes - 1][0], STDIN_FILENO);
	}
	else
	{
		ft_dup2(db, pipes[index - 1][0], STDIN_FILENO);
		ft_dup2(db, pipes[index][1], STDOUT_FILENO);
	}
	return (SUCCESS);
}

int	close_all_pipes(t_db *db, int **pipes)
{
	int	pipe_i;
	int	n_pipes;

	n_pipes = get_pipes_count(pipes);
	pipe_i = 0;
	if (!pipes || !pipes[0])
		return (SUCCESS);
	while (pipe_i < n_pipes)
	{
		ft_close(db, &pipes[pipe_i][0]);
		ft_close(db, &pipes[pipe_i][1]);
		pipe_i++;
	}
	return (SUCCESS);
}

int	dup_cmd_io(t_db *db, t_cmd_node *command)
{
	if (command->input_fd != INVALID && command->input_fd != STDIN_FILENO)
	{
		ft_dup2(db, command->input_fd, STDIN_FILENO);
		ft_close(db, &command->input_fd);
	}
	if (command->output_fd != INVALID && command->output_fd != STDOUT_FILENO)
	{
		ft_dup2(db, command->output_fd, STDOUT_FILENO);
		ft_close(db, &command->output_fd);
	}
	return (SUCCESS);
}

int	get_pipes_count(int **pipes)
{
	int	i;

	i = 0;
	if (!pipes || !pipes[0])
		return (-1);
	while (pipes[i])
		i++;
	return (i);
}
