#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "parsing.h"


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

void	handle_underscore(t_db *db, void *node)
{
	t_env_list	*_;
	char		*last_arg;

	_ = get_env_node(db->env_list, "_");
	if (_)
		ec_free(db, _->val);
	last_arg = CMD->args[count_array_len(CMD->args) - 1];
	if (_)
		_->val = ft_strdup_ec(db, last_arg);
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
