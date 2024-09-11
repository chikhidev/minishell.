/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 22:07:30 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 22:54:12 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

char	**env_list_to_env_arr(t_db *db)
{
	t_env_list	*env_list;
	char		**env_arr;
	int			env_len;
	char		*temp;

	env_len = 0;
	env_arr = NULL;
	env_list = db->env_list;
	while (env_list && (++env_len))
		env_list = env_list->next;
	env_arr = ec_malloc(db, sizeof(char *) * (env_len + 1));
	env_list = db->env_list;
	env_len = 0;
	while (env_list)
	{
		env_arr[env_len] = ec_malloc(db, ft_strlen(env_list->key) + 1
				+ ft_strlen(env_list->val) + 1);
		temp = ft_strjoin(db, env_list->key, "=");
		temp = ft_strjoin(db, temp, env_list->val);
		env_arr[env_len++] = temp;
		env_list = env_list->next;
	}
	env_arr[env_len] = NULL;
	return (env_arr);
}

t_env_list	*set_default_env(t_db *db)
{
	t_env_list	*new;

	db->static_path = "/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin";
	new = new_env_node(db, "PWD", getcwd(NULL, 0));
	push_env_back(&db->env_list, new);
	new = new_env_node(db, "SHLVL", "0");
	push_env_back(&db->env_list, new);
	new = new_env_node(db, "_", "/usr/bin/env");
	push_env_back(&db->env_list, new);
	return (db->env_list);
}
