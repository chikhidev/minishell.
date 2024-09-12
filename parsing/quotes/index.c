/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:36 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/12 07:28:15 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "string.h"

void	update_quotes(t_quote *head, int start, int old_len, int new_len)
{
	t_quote	*q;

	if (!head)
		return ;
	q = head;
	while (q)
	{
		if (start > q->start && start < q->end)
		{
			q->end += new_len - old_len;
		}
		else if (q->start > start)
		{
			q->start += new_len - old_len;
			q->end += new_len - old_len;
		}
		q = q->next;
	}
}

void	handle_delete_quote(t_db *db, t_quote **head, t_quote *prev, t_quote *q)
{
	if (prev)
		prev->next = q->next;
	else
		*head = q->next;
	tmp = q;
	if (prev)
		q = prev->next;
	else
		q = *head;
	gc_free(db, tmp);
}

void	delete_quotes_in_range(t_quote **head, int start, int end)
{
	t_quote	*q;
	t_quote	*prev;
	t_quote	*tmp;
	t_db	*db;

	db = this();
	if (!*head)
		return ;
	q = *head;
	prev = NULL;
	while (q)
	{
		if (q->start >= start && q->end <= end)
		{
			handle_delete_quote(db, head, prev, q);
		}
		else
		{
			prev = q;
			q = q->next;
		}
	}
}

t_quote	*quote_at(t_quote *head, int index)
{
	t_quote	*q;

	if (!head)
		return (NULL);
	q = head;
	while (q)
	{
		if (index == q->start || index == q->end)
			return (q);
		q = q->next;
	}
	return (NULL);
}

t_quote	*quotes_of_range(t_quote *head, int pos)
{
	t_quote	*q;

	if (!head)
		return (NULL);
	q = head;
	while (q)
	{
		if (pos >= q->start && pos <= q->end)
			return (q);
		q = q->next;
	}
	return (NULL);
}
