/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 22:39:18 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/13 12:02:57 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	handle_sigint(int signum)
{
	t_db	*db;

	db = this();
	(void)signum;
	ft_write(db, STDOUT_FILENO, "\n", 1);
	db->last_status = 130;
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	handle_prompt(t_db *db, char **line)
{
	char				*tmp;

	signal(SIGINT, handle_sigint);
	signal(SIGQUIT, SIG_IGN);
	tmp = ft_strjoin(db, "\001" ORANGE "\002>_\001" RESET "\002", "$ ");
	*line = readline(tmp);
	if (!*line)
	{
		put_fd(2, "exit\n");
		return (FAILURE);
	}
	tmp = ft_strdup(db, *line);
	free(*line);
	*line = tmp;
	if (ft_strlen(*line) > 0)
		add_history(*line);
	return (SUCCESS); /*nothing*/
}

void	handle_shell_level(t_db *db, char *key, char **val)
{
	int	shlvl;

	if (ft_strcmp(key, "SHLVL") == 0)
	{
		shlvl = ft_atoi((*val)) + 1;
		if (shlvl >= 1000)
			shlvl = 1;
		(*val) = ft_itoa_ec(db, shlvl);
	}
}

t_env_list	*set_env_lst(t_db *db, char *env[])
{
	t_env_list	*env_list;
	t_env_list	*new_node;
	int			i;
	char		*key;
	char		*val;

	env_list = NULL;
	new_node = NULL;
	i = 0;
	key = NULL;
	val = NULL;
	if (env == NULL || !env[0])
		return (set_default_env(db));
	while (env && env[i])
	{
		fill_key_val(db, env[i], &key, &val);
		handle_shell_level(db, key, &val);
		new_node = new_env_node(db, key, val);
		push_env_back(&env_list, new_node);
		i++;
	}
	return (env_list);
}

void	handle_pwd(t_db *db)
{
	t_exp_list	*exp;
	t_exp_list	*pwd_exp;
	t_exp_list	*old_pwd_exp;
	t_exp_list	*underscore_exp;
	t_env_list	*underscore_env;
	t_env_list	*pwd_env;
	bool		pwd_exist;
	bool		old_pwd_exist;
	bool		underscore_exist;

	pwd_exist = false;
	underscore_exist = false;
	old_pwd_exist = false;
	exp = db->exp_list;
	while (exp)
	{
		if (ft_strcmp(exp->key, "PWD") == 0)
			pwd_exist = true;
		if (ft_strcmp(exp->key, "OLDPWD") == 0)
			old_pwd_exist = true;
		if (ft_strcmp(exp->key, "_") == 0)
			underscore_exist = true;
		exp = exp->next;
	}
	if (!pwd_exist)
	{
		pwd_exp = new_exp_node(db, ft_strdup_ec(db, "PWD"), ft_strdup_ec(db,
					getcwd(NULL, 0)));
		push_exp_sort(&db->exp_list, pwd_exp);
		pwd_env = new_env_node(db, ft_strdup_ec(db, "PWD"), ft_strdup_ec(db,
					getcwd(NULL, 0)));
		push_env_back(&db->env_list, pwd_env);
	}
	if (!old_pwd_exist)
	{
		old_pwd_exp = new_exp_node(db, ft_strdup_ec(db, "OLDPWD"), NULL);
		push_exp_sort(&db->exp_list, old_pwd_exp);
	}
	if (!underscore_exist)
	{
		underscore_exp = new_exp_node(db, ft_strdup_ec(db, "_"),
				ft_strdup_ec(db, "./minishell"));
		push_exp_sort(&db->exp_list, underscore_exp);
		underscore_env = new_env_node(db, ft_strdup_ec(db, "_"),
				ft_strdup_ec(db, "./minishell"));
		push_env_back(&db->env_list, underscore_env);
	}
}

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

int	main(int ac, char *av[], char *env[])
{
	t_db				*db;
	char				*line;
	char				*tmp;
	int					ret;
	struct sigaction	sa;

	db = this();
	ft_bzero(&sa, sizeof(struct sigaction));
	line = NULL;
	init_db(db, ac, av, env);
	while (true)
	{
		db_reset(db);
		ret = handle_prompt(db, &line);
		if (ret == FAILURE)
			break ;
		if (ret == 0)
			continue ;
		sa.sa_handler = SIG_IGN;
		sigaction(SIGINT, &sa, NULL);
		sigaction(SIGQUIT, &sa, NULL);
		tmp = gc_malloc(db, ft_strlen(line) + 1);
		ft_strlcpy(tmp, line, ft_strlen(line) + 1);
		if (parser(db, line) == SUCCESS)
			exec(db, db->root_node);
		fd_void(db);
		gc_void(db);
		pid_void(db);
	}
	fd_void(db);
	ec_void(db);
	gc_void(db);
	close(0);
	close(1);
	close(2);
	return (db->last_status);
}
