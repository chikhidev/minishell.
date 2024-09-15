/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 22:39:18 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/15 01:39:45 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	init_db(t_db *db, int ac, char *av[], char *env[])
{
	(void)ac;
	(void)av;
	db->last_status = 0;
	db->debug = false;
	db->gc = NULL;
	db->ec = NULL;
	db->error = false;
	db->env = env;
	db->env_list = NULL;
	db->exp_list = NULL;
	db->static_path = NULL;
	db->env_list = set_env_lst(db, env);
	db->exp_list = set_exp_lst(db, env);
	handle_pwd(db);
	db->pid = NULL;
	db->fd = NULL;
	db->stdin_dup = -1;
	db->stdout_dup = -1;
}

void	db_reset(t_db *db)
{
	db->exec_line = true;
	db->heredoc_counter = 0;
	db->root_node = NULL;
	db->error = false;
	db->curr_type = INVALID;
	db->input_fd = STDIN_FILENO;
	db->output_fd = STDOUT_FILENO;
	db->fd = NULL;
	db->split = false;
	pid_void(db);
}

t_db	*this(void)
{
	static t_db	db;

	return (&db);
}

int	shell_routine(void)
{
	int		ret;
	char	*tmp;
	char	*line;

	line = NULL;
	ret = handle_prompt(this(), &line);
	if (ret == FAILURE)
		return (FAILURE);
	if (ret == 0)
		return (SUCCESS);
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	tmp = gc_malloc(this(), (int)len(line) + 1);
	ft_strlcpy(tmp, line, (int)len(line) + 1);
	if (parser(this(), line) == SUCCESS)
		exec(this(), this()->root_node);
	fd_void(this());
	gc_void(this());
	pid_void(this());
	return (SUCCESS);
}

int	main(int ac, char *av[], char *env[])
{
	t_db				*db;

	if (ac > 2)
		return (1);
	db = this();
	init_db(db, ac, av, env);
	while (true)
	{
		db_reset(db);
		if (shell_routine() == FAILURE)
			break ;
	}
	fd_void(db);
	ec_void(db);
	gc_void(db);
	close(0);
	close(1);
	close(2);
	return (db->last_status);
}
