/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:34 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 23:36:50 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

bool	inside_single_quote(t_quote *head, int i)
{
	t_quote	*q;

	if (!head)
		return (false);
	q = head;
	while (q)
	{
		if (q->ascii == SNGLQUOTE && i > q->start && i < q->end)
			return (true);
		q = q->next;
	}
	return (false);
}

bool	is_inside_quotes_list(t_quote *head, int i)
{
	t_quote	*tmp;

	if (!head)
		return (false);
	tmp = head;
	while (tmp)
	{
		if (i > tmp->start && i < tmp->end)
		{
			return (true);
		}
		tmp = tmp->next;
	}
	return (false);
}

bool	is_quote_oppening(t_quote *head, int i)
{
	t_quote	*q;

	if (!head)
		return (false);
	q = head;
	while (q)
	{
		if (q->start == i)
			return (true);
		q = q->next;
	}
	return (false);
}

bool	is_quote(t_quote *head, int i)
{
	t_quote	*q;

	if (!head)
		return (false);
	q = head;
	while (q)
	{
		if (i == q->start || i == q->end)
			return (true);
		q = q->next;
	}
	return (false);
}
