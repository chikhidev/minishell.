/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:08:33 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 20:28:37 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	handle_add_env_export(t_db *db, char *key, char *val)
{
	t_env_list	*env_node;

	key = ft_strdup_ec(db, key);
	val = ft_strdup_ec(db, val);
	env_node = new_env_node(db, key, val);
	push_env_back(&db->env_list, env_node);
}

void	handle_add_exp_export(t_db *db, char *key, char *val)
{
	t_exp_list	*exp_node;

	key = ft_strdup_ec(db, key);
	val = ft_strdup_ec(db, val);
	exp_node = new_exp_node(db, key, val);
	push_exp_sort(&db->exp_list, exp_node);
}

void	put_exp_err_status(char *arg, int *status)
{
	put_fd(2, "export: `");
	put_fd(2, arg);
	put_fd(2, "': not a valid identifier\n");
	*status = 1;
}

void	assign_append_exp(t_db *db, char *val, char *token, bool *append)
{
	int			k_len;
	int			v_len;
	char		*key;
	t_exp_list	*exp_node;
	t_env_list	*env_node;

	k_len = 0;
	v_len = 0;
	*append = false;
	key = get_key_from_arg(db, token, &k_len, append);
	exp_node = get_exp_node(db->exp_list, key);
	env_node = get_env_node(db->env_list, key);
	val = get_val_from_arg(token, &v_len, k_len, *append);
	if (exp_node)
		affect_exp_node_val(db, exp_node, *append, val);
	else
		handle_add_exp_export(db, key, val);
	if (env_node)
		affect_env_node_val(db, env_node, *append, val);
	else
		handle_add_env_export(db, key, val);
}

void	add_exp(char *key, char *val)
{
	t_db		*db;
	t_exp_list	*exp_node;

	db = this();
	exp_node = get_exp_node(db->exp_list, key);
	if (!exp_node)
	{
		if (ft_strcmp(key, "PATH") == 0 && db->static_path)
			val = db->static_path;
		exp_node = new_exp_node(db, key, val);
		push_exp_sort(&db->exp_list, exp_node);
	}
}
