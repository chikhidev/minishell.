#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "parsing.h"

void	ft_exit(t_db *db, int status, short free_flag, char *msg)
{
	if (msg)
		dprintf(2, "%s\n", msg);
	if (free_flag > 2)
		fd_void(db);
	if (free_flag > 1)
		ec_void(db);
	if (free_flag > 0)
		gc_void(db);
	exit(status);
}

void	ft_write(t_db *db, int fd, char *msg, int len)
{
	int	res;

	res = write(fd, msg, len);
	if (res == -1)
		ft_exit(db, 1, 3, "write failed");
}

void	ft_close(t_db *db, int *fd)
{
	int	res;

	res = 0;
	if (*fd != CLOSED)
	{
		res = close(*fd);
		fd_free(db, *fd);
	}
	if (res != INVALID)
		return ;
	ft_exit(db, 1, 3, "close failed");
}

void	ft_pipe(t_db *db, int *pipe_fd)
{
	int	res;

	res = pipe(pipe_fd);
	if (res != INVALID && pipe_fd[0] != INVALID)
		fd_add(db, pipe_fd[0]);
	if (res != INVALID && pipe_fd[1] != INVALID)
		fd_add(db, pipe_fd[1]);
	if (res != INVALID)
		return ;
	ft_exit(db, 1, 3, "pipe failed");
}

void	ft_dup2(t_db *db, int old_fd, int new_fd)
{
	int	res;

	res = dup2(old_fd, new_fd);
	if (old_fd != INVALID)
		fd_add(db, old_fd);
	if (res != INVALID)
		return ;
	ft_exit(db, 1, 3, "dup2 failed");
}

int	ft_dup(t_db *db, int fd)
{
	int	new_fd;

	new_fd = dup(fd);
	if (fd != INVALID)
		fd_add(db, new_fd);
	if (new_fd != INVALID)
		return (new_fd);
	ft_exit(db, 1, 3, "dup failed");
	return (new_fd);
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
		if (access(path, F_OK) + access(path, X_OK) != 0)
		{
			perror(path);
			fd_void(db);
			ec_void(db);
			exit(error(db, NULL, NULL) + 127 - (access(path, X_OK) != 0));
		}
	}
	else
	{
		path = cmd_path(db, args[0]);
		if (db->error)
		{
			fd_void(db);
			exit(126);
		}
		if (!path)
			ft_exit(db, 127, 3, ft_strjoin(db, args[0], ": command not found"));
	}
	return (path);
}

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

void	waiter(t_db *db)
{
	int		status;
	t_int	*pid;

	status = 0;
	pid = db->pid;
	while (pid)
	{
		waitpid(pid->n, &status, 0);
		pid = pid->next;
	}
	catch_feedback(db, status);
	pid_void(db);
}

int	dup_pipes(t_db *db, int **pipes, int index) // index -> 2
{
	if (index == -1)
		return (SUCCESS);
	int n_pipes = get_pipes_count(pipes);
	if (index == 0)
		ft_dup2(db, pipes[0][1], STDOUT_FILENO);
	else if (index == n_pipes) // 2 == 2
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

void	handle_pipe_op(t_db *db, void *node)
{
	int	i;
	int	**pipes;

	i = 0;
	pipes = prepare_pipes(db, OP->n_childs - 1);
	while (i < OP->n_childs)
	{
		handle_cmd_node(db, OP->childs[i], pipes, i);
		i++;
	}
	close_all_pipes(db, pipes);
	waiter(db);
	return ;
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

void	handle_is_dir(t_db *db, char *arg)
{
	DIR	*dir;

	(void)db;
	if (ft_strcmp(arg, ".") == 0)
		ft_exit(db, 2, 3, ft_strjoin(db, arg, ": command not found"));
	if (ft_strcmp(arg, "..") == 0)
	{
		ft_exit(db, 127, 3, ft_strjoin(db, arg, ": command not found"));
	}
	dir = opendir(arg);
	if (dir)
	{
		closedir(dir);
		ft_exit(db, 126, 3, ft_strjoin(db, arg, ": Is a directory"));
	}
}

void	exec_cmd(t_db *db, void *node, int **pipes, int index)
{
	char	**env_arr;
	char	*path;

	if (CMD->input_fd == INVALID || CMD->output_fd == INVALID)
		ft_exit(db, 1, 3, NULL);
	path = get_path(db, CMD->args);
	env_arr = env_list_to_env_arr(db);
	dup_pipes(db, pipes, index);
	close_all_pipes(db, pipes);
	dup_cmd_io(db, node);
	handle_is_dir(db, CMD->args[0]);
	execve(path, CMD->args, env_arr);
	ft_exit(db, 127, 3, ft_strjoin(db, path, ": failed"));
}

int	handle_builtin(t_db *db, void *node, int **pipes, int index)
{
	int	in;
	int	out;
	int	id;

	if (CMD->input_fd == INVALID || CMD->output_fd == INVALID)
		return (db->last_status = 1, 1);
	if (index == -1)
	{
		in = ft_dup(db, STDIN_FILENO);
		out = ft_dup(db, STDOUT_FILENO);
		dup_cmd_io(db, node);
		db->last_status = run_builtin(db, node, index);
		dup2(in, STDIN_FILENO);
		dup2(out, STDOUT_FILENO);
		return (db->last_status);
	}
	id = fork();
	if (id == CHILD)
	{
		dup_pipes(db, pipes, index);
		dup_cmd_io(db, node);
		close_all_pipes(db, pipes);
		db->last_status = run_builtin(db, node, index);
		db->last_status = db->last_status << 8;
		ft_exit(db, db->last_status, 3, NULL);
	}
	else
	{
		pid_add(db, id);
	}
	return (db->last_status);
}

void	handle_cmd_node(t_db *db, void *node, int **pipes, int index)
{
	int	id;
	int	status;

	if (!node || !CMD->args || !CMD->args[0])
		return ;

	handle_underscore(db, node);
	status = 0;
	if (is_built_in(node))
		handle_builtin(db, node, pipes, index);
	else
	{
		id = fork();
		if (id == CHILD)
		{
			default_signals_behav(false);
			exec_cmd(db, node, pipes, index);
		}
		else
		{
			if (index == -1)
			{
				waitpid(id, &status, 0);
				catch_feedback(db, status);
			}
			else
				pid_add(db, id);
		}
	}
}

void	handle_underscore(t_db *db, void *node)
{
	t_env_list	*_;
	char		*last_arg;

	_ = get_env_node(db->env_list, "_");
	ec_free(db, _->val);
	last_arg = CMD->args[count_array_len(CMD->args) - 1];
	_->val = ft_strdup_ec(db, last_arg);
}

void	exec(t_db *db, void *node)
{
	if (!node)
		return ;
	handle_parent_signals();
	if (CMD->type == CMD_NODE)
		handle_cmd_node(db, node, NULL, -1);
	else if (OP->op_presentation == PIPE)
		handle_pipe_op(db, node);
}
