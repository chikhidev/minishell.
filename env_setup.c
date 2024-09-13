/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_setup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 17:11:13 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/12 17:11:14 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	handle_pwd_oldpwd_no_exist(t_db *db, bool pwd_exist, bool old_pwd_exist)
{
	t_exp_list	*pwd_exp;
	t_exp_list	*old_pwd_exp;
	t_env_list	*pwd_env;

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
}

void	set_exist_bool(t_exp_list *exp, bool *pwd_exist, bool *old_pwd_exist,
		bool *underscore_exist)
{
	if (ft_strcmp(exp->key, "PWD") == 0)
		*pwd_exist = true;
	if (ft_strcmp(exp->key, "OLDPWD") == 0)
		*old_pwd_exist = true;
	if (ft_strcmp(exp->key, "_") == 0)
		*underscore_exist = true;
}

void	handle_underscore_no_exist(t_db *db)
{
	t_exp_list	*underscore_exp;
	t_env_list	*underscore_env;

	underscore_exp = new_exp_node(db, ft_strdup_ec(db, "_"), ft_strdup_ec(db,
				"./minishell"));
	push_exp_sort(&db->exp_list, underscore_exp);
	underscore_env = new_env_node(db, ft_strdup_ec(db, "_"), ft_strdup_ec(db,
				"./minishell"));
	push_env_back(&db->env_list, underscore_env);
}

void	handle_pwd(t_db *db)
{
	t_exp_list	*exp;
	bool		pwd_exist;
	bool		old_pwd_exist;
	bool		underscore_exist;

	pwd_exist = false;
	underscore_exist = false;
	old_pwd_exist = false;
	exp = db->exp_list;
	while (exp)
	{
		set_exist_bool(exp, &pwd_exist, &old_pwd_exist, &underscore_exist);
		exp = exp->next;
	}
	if (!underscore_exist)
		handle_underscore_no_exist(db);
	handle_pwd_oldpwd_no_exist(db, pwd_exist, old_pwd_exist);
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
