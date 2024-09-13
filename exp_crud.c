/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_crud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 22:38:39 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/13 16:57:04 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	put_in_middle_exp(t_exp_list **list, t_exp_list *new)
{
	t_exp_list	*prev;
	t_exp_list	*curr;

	prev = *list;
	curr = prev->next;
	while (curr && curr->next)
	{
		if (ft_strncmp(new->key, curr->key, ft_strlen(curr->key)) < 0)
		{
			prev->next = new;
			new->next = curr;
			return ;
		}
		prev = curr;
		curr = curr->next;
	}
	if (curr)
		curr->next = new;
	else
		add_exp_front(list, new);
}

void	push_exp_sort(t_exp_list **list, t_exp_list *new)
{
	if (!list)
		return ;
	if (*list == NULL)
		add_exp_front(list, new);
	else if (ft_strncmp(new->key, (*list)->key, ft_strlen((*list)->key)) < 0)
		add_exp_front(list, new);
	else
		put_in_middle_exp(list, new);
}

void	push_exp_back(t_exp_list **list, t_exp_list *new)
{
	t_exp_list	*curr;

	if (list)
	{
		if (*list == NULL)
		{
			add_exp_front(list, new);
		}
		else if (ft_strncmp(new->key, (*list)->key,
				ft_strlen((*list)->key)) < 0)
			add_exp_front(list, new);
		else
		{
			curr = (*list);
			while (curr && curr->next)
				curr = curr->next;
			curr->next = new;
		}
	}
}

t_exp_list	*set_default_exp(t_db *db)
{
	t_exp_list	*new;

	new = new_exp_node(db, "OLDPWD", NULL);
	push_exp_back(&db->exp_list, new);
	new = new_exp_node(db, "PWD", getcwd(NULL, 0));
	push_exp_back(&db->exp_list, new);
	new = new_exp_node(db, "SHLVL", "0");
	push_exp_back(&db->exp_list, new);
	new = new_exp_node(db, "_", "/usr/bin/exp");
	push_exp_back(&db->exp_list, new);
	return (db->exp_list);
}

t_exp_list	*set_exp_lst(t_db *db, char *env[])
{
	t_exp_list	*exp_list;
	t_exp_list	*new_node;
	int			i;
	char		*key;
	char		*val;

	exp_list = NULL;
	key = NULL;
	val = NULL;
	i = 0;
	if (env == NULL || !env[0])
		return (set_default_exp(db));
	while (env && env[i])
	{
		fill_key_val(db, env[i], &key, &val);
		handle_shell_level(db, key, &val);
		new_node = new_exp_node(db, key, val);
		new_node->next = NULL;
		push_exp_sort(&exp_list, new_node);
		i++;
	}
	return (exp_list);
}
