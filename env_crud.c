/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_crud.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 22:06:50 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 22:11:50 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

t_env_list	*new_env_node(t_db *db, char *key, char *val)
{
	t_env_list	*new;

	new = ec_malloc(db, sizeof(t_env_list));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->key = key;
	new->val = val;
	return (new);
}

void	add_env_front(t_env_list **list, t_env_list *new)
{
	if (list && new)
	{
		new->next = *list;
		*list = new;
	}
}

void	push_env_back(t_env_list **list, t_env_list *new)
{
	t_env_list	*last;

	if (list)
	{
		if (*list == NULL)
			add_env_front(list, new);
		else
		{
			last = *list;
			while (last && last->next)
				last = last->next;
			last->next = new;
		}
	}
}

t_env_list	*get_env_node(t_env_list *list, char *key)
{
	t_env_list	*curr;

	curr = list;
	while (curr)
	{
		if (ft_strcmp(curr->key, key) == 0)
			return (curr);
		curr = curr->next;
	}
	return (curr);
}

void	del_env_node(t_env_list **list, char *key)
{
	t_env_list	*curr;
	t_env_list	*prev;

	prev = *list;
	if (prev)
	{
		if (ft_strcmp(prev->key, key) == 0)
		{
			*list = (*list)->next;
			return ;
		}
		curr = prev->next;
		while (curr)
		{
			if (ft_strcmp(curr->key, key) == 0)
			{
				prev->next = curr->next;
				return ;
			}
			prev = curr;
			curr = curr->next;
		}
	}
}
