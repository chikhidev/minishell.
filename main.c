/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 22:39:18 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/12 21:32:25 by abchikhi         ###   ########.fr       */
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

int routine(char **line)
{
	int		ret;
	char	*tmp;

	db_reset(this());
	ret = handle_prompt(this(), line);
	if (ret == FAILURE)
		return (FAILURE);
	if (ret == 0)
		return (SUCCESS);
	tmp = gc_malloc(this(), ft_strlen(*line) + 1);
	ft_strlcpy(tmp, *line, ft_strlen(*line) + 1);
	if (parser(this(), *line) == SUCCESS)
		exec(this(), this()->root_node);
	return (SUCCESS);
}

int	main(int ac, char *av[], char *env[])
{
	t_db				*db;
	char				*line;

	db = this();
	line = NULL;
	init_db(db, ac, av, env);
	while (true)
	{
		if (routine(&line) == FAILURE)
			break ;
		fd_void(db);
		gc_void(db);
		pid_void(db);
	}
	fd_void(db);
	ec_void(db);
	gc_void(db);
	(close(0), close(1), close(2));
	return (db->last_status);
}
