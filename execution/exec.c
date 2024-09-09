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
		if (access(path, F_OK) + access(path, X_OK) != 0)
		{
            (perror(path), fd_void(db), ec_void(db));
			exit(error(db, NULL, NULL) + 127 - (access(path, X_OK) != 0));
		}
	}
	else
	{
		path = cmd_path(db, args[0]);
		if (db->error)
			(fd_void(db), exit(126));
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
