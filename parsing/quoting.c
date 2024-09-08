#include "main.h"

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
		else if ((line[i] == SNGLQUOTE || line[i] == DOBLQUOTE) && last->end !=
			-1)
			add_quote(db, head, line[i], i);
		i++;
	}
	last = last_quote(*head);
	if (last && last->end == -1)
	{
		printf("Quotes are not closed expecting %c\n", last->ascii);
		return (error(db, NULL, NULL));
	}
	return (SUCCESS);
}

void	update_quotes(t_quote *head, int start, int old_len, int new_len)
{
	t_quote	*q;

	if (!head)
		return ;
	q = head;

	// printf(MAGENTA"preparing to update quotes with {old_len: %d, new_len: %d}\n"RESET, old_len, new_len);

	while (q)
	{
		if (start > q->start && start < q->end)
		{
			// printf(RED"updated {%d, %d} to ", q->start, q->end);
			q->end += new_len - old_len;
			// printf("{%d, %d}\n"RESET, q->start, q->end);
		}
		else if (q->start > start)
		{
			// printf(RED"updated {%d, %d} to ", q->start, q->end);
			q->start += new_len - old_len;
			q->end += new_len - old_len;
			// printf("{%d, %d}\n"RESET, q->start, q->end);
		}
		q = q->next;
	}
}

void	delete_quotes_in_range(t_quote **head, int start, int end)
{
	t_quote	*q;
	t_quote	*prev;
	t_quote	*tmp;

	t_db *db = this();

	if (!*head)
		return ;
	q = *head;
	prev = NULL;
	while (q)
	{
		if (q->start >= start && q->end <= end)
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

			// printf(RED"deleted {%d, %d}\n"RESET, tmp->start, tmp->end);
			gc_free(db, tmp);

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
	t_quote *q;

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

t_quote *quotes_of_range(t_quote *head, int pos)
{
	t_quote *q;

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
