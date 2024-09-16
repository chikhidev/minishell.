/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:27:14 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/16 20:46:12 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

int	add_quote(t_db *db, t_quote **head, int ascii, int start)
{
	t_quote	*new;
	t_quote	*tmp;

	new = gc_malloc(db, sizeof(t_quote));
	new->ascii = ascii;
	new->start = start;
	new->end = -1;
	new->next = NULL;
	if (!*head)
	{
		*head = new;
		return (SUCCESS);
	}
	tmp = *head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (SUCCESS);
}

t_quote	*last_quote(t_quote *head)
{
	t_quote	*tmp;

	if (!head)
		return (NULL);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

void	handle_quote_error(t_quote *last)
{
	put_fd(2, "Quotes are not closed expecting: ");
	if (last->ascii == SNGLQUOTE)
		put_fd(2, "'");
	else
		put_fd(2, "\"");
	put_fd(2, "\n");
}

int	track_quotes(t_db *db, t_quote **head, char *line)
{
	int		i;
	t_quote	*last;

	i = 0;
	while (line[i])
	{
		last = last_quote(*head);
		if ((line[i] == SNGLQUOTE || line[i] == DOBLQUOTE) && !last)
			add_quote(db, head, line[i], i);
		else if ((line[i] == SNGLQUOTE || line[i] == DOBLQUOTE)
			&& last->ascii == line[i] && last->end == -1)
		{
			last->end = i;
		}
		else if ((line[i] == SNGLQUOTE || line[i] == DOBLQUOTE)
			&& last->end != -1)
			add_quote(db, head, line[i], i);
		i++;
	}
	last = last_quote(*head);
	if (last && last->end == -1)
		return (handle_quote_error(last), error(db, NULL, NULL));
	return (SUCCESS);
}
