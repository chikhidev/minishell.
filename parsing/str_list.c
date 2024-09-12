/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_list.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:42 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/12 02:48:43 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/exec.h"
#include "../includes/main.h"
#include "../includes/parsing.h"
#include "../includes/string.h"

t_str_lst	*new_str_node(t_db *db, char *str)
{
	t_str_lst	*new;

	new = gc_malloc(db, sizeof(t_str_lst));
	if (new == NULL)
		return (NULL);
	new->next = NULL;
	new->str = str;
	return (new);
}
void	add_str_front(t_str_lst **list, t_str_lst *new)
{
	if (list && new)
	{
		new->next = *list;
		*list = new;
	}
}
void	push_str_back(t_str_lst **list, t_str_lst *new)
{
	t_str_lst	*last;

	if (list)
	{
		if (*list == NULL)
			add_str_front(list, new);
		else
		{
			last = *list;
			while (last && last->next)
				last = last->next;
			last->next = new;
		}
	}
}
t_str_lst	*get_str_node(t_str_lst *list, char *str)
{
	t_str_lst	*curr;

	curr = list;
	while (curr)
	{
		if (ft_strcmp(curr->str, str) == 0)
			return (curr);
		curr = curr->next;
	}
	return (curr);
}

void	del_str_node(t_str_lst **list, char *str)
{
	t_str_lst *curr;
	t_str_lst *prev;

	prev = *list;
	if (prev)
	{
		if (ft_strcmp(prev->str, str) == 0)
		{
			*list = (*list)->next;
			return ;
		}
		curr = prev->next;
		while (curr)
		{
			if (ft_strcmp(curr->str, str) == 0)
			{
				prev->next = curr->next;
				return ;
			}
			prev = curr;
			curr = curr->next;
		}
	}
	return ;
}