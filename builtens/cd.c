/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 15:51:18 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/16 03:01:29 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	set_pwd_str_values(t_db *db, char **pwd_str, char **old_pwd_str)
{
	t_env_list	*pwd_env;
	t_env_list	*old_pwd_env;

	pwd_env = get_env_node(db->env_list, "PWD");
	old_pwd_env = get_env_node(db->env_list, "OLDPWD");
	if (pwd_env)
		*pwd_str = ft_strdup_ec(db, pwd_env->val);
	if (old_pwd_env)
		*old_pwd_str = ft_strdup_ec(db, old_pwd_env->val);
}

void	set_pwd_exp_values(t_db *db, char *old_pwd_str)
{
	t_exp_list	*pwd_exp;
	t_exp_list	*old_pwd_exp;
	char		*new_pwd;

	new_pwd = getcwd(NULL, 0);
	pwd_exp = get_exp_node(db->exp_list, ft_strdup_ec(db, "PWD"));
	old_pwd_exp = get_exp_node(db->exp_list, ft_strdup_ec(db, "OLDPWD"));
	if (pwd_exp)
		pwd_exp->val = ft_strdup_ec(db, new_pwd);
	else
		push_exp_sort(&db->exp_list, new_exp_node(db, ft_strdup_ec(db, "PWD"),
				ft_strdup_ec(db, new_pwd)));
	if (old_pwd_exp)
		old_pwd_exp->val = ft_strdup_ec(db, old_pwd_str);
	else
		push_exp_sort(&db->exp_list, new_exp_node(db, ft_strdup_ec(db,
					"OLDPWD"), ft_strdup_ec(db, old_pwd_str)));
	free(new_pwd);
}

void	set_pwd_env_values(t_db *db, char *old_pwd_str)
{
	t_env_list	*pwd_env;
	t_env_list	*old_pwd_env;
	char		*new_pwd;

	new_pwd = getcwd(NULL, 0);
	pwd_env = get_env_node(db->env_list, ft_strdup_ec(db, "PWD"));
	old_pwd_env = get_env_node(db->env_list, ft_strdup_ec(db, "OLDPWD"));
	if (pwd_env)
		pwd_env->val = ft_strdup_ec(db, new_pwd);
	else
		push_env_back(&db->env_list, new_env_node(db, ft_strdup_ec(db, "PWD"),
				ft_strdup_ec(db, new_pwd)));
	if (old_pwd_env)
		old_pwd_env->val = ft_strdup_ec(db, old_pwd_str);
	else
		push_env_back(&db->env_list, new_env_node(db, ft_strdup_ec(db,
					"OLDPWD"), ft_strdup_ec(db, old_pwd_str)));
	free(new_pwd);
}

int	going_home(t_db *db, char **dest)
{
	t_exp_list	*home_exp;
	char		*buff;

	home_exp = get_exp_node(db->exp_list, "HOME");
	if (home_exp)
	{
		*dest = home_exp->val;
		if (*dest && !**dest)
		{
			buff = getcwd(NULL, 0);
			*dest = ft_strdup_ec(db, buff);
			free(buff);
		}
		return (0);
	}
	else
	{
		return (put_fd(2, "cd: HOME not set\n"), 1);
	}
	return (0);
}

int	cd_(t_db *db, char *args[])
{
	char	*dest;
	char	*pwd_str;
	char	*oldpwd_str;
	int		status;

	if (count_array_len(args) > 2)
		return (put_fd(2, "cd : too many arguments\n"), 1);
	status = 0;
	pwd_str = NULL;
	oldpwd_str = NULL;
	dest = NULL;
	set_pwd_str_values(db, &pwd_str, &oldpwd_str);
	if (count_array_len(args) == 1)
		status = going_home(db, &dest);
	else if (is_str_empty(db, args[1]))
		return (0);
	else
		dest = args[1];
	if (status)
		return (status);
	if (chdir(dest) == -1)
		return (put_fd(2, "cd: "), perror(dest), 1);
	set_pwd_env_values(db, pwd_str);
	set_pwd_exp_values(db, pwd_str);
	return (0);
}
