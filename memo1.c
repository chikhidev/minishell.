/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memo1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 02:09:24 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/13 16:55:39 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/main.h"
#include "includes/string.h"

void	gc_void(t_db *db)
{
	t_gc	*gc;

	while (db->gc)
	{
		gc = db->gc;
		db->gc = db->gc->next;
		free(gc->ptr);
		free(gc);
	}
	db->gc = NULL;
}

void	ec_void(t_db *db)
{
	t_gc	*ec;

	while (db->ec)
	{
		ec = db->ec;
		db->ec = db->ec->next;
		free(ec->ptr);
		free(ec);
	}
	db->ec = NULL;
}

void	*gc_realloc(t_db *db, void *ptr, size_t old_size, size_t new_size)
{
	t_gc	*gc;
	void	*new_ptr;

	gc = db->gc;
	while (gc)
	{
		if (gc->ptr == ptr)
		{
			new_ptr = malloc(new_size);
			if (!new_ptr)
			{
				gc_void(db);
				fd_void(db);
				ec_void(db);
				exit(FAIL);
			}
			ft_bzero(new_ptr, new_size);
			ft_memcpy(new_ptr, ptr, old_size);
			free(ptr);
			gc->ptr = new_ptr;
			return (new_ptr);
		}
		gc = gc->next;
	}
	return (NULL);
}

void	ec_free(t_db *db, void *ptr)
{
	t_gc	*ec;
	t_gc	*prev;

	ec = db->ec;
	prev = NULL;
	while (ec)
	{
		if (ec->ptr == ptr)
		{
			if (prev)
				prev->next = ec->next;
			else
				db->ec = ec->next;
			free(ec->ptr);
			free(ec);
			return ;
		}
		prev = ec;
		ec = ec->next;
	}
}

void	gc_free(t_db *db, void *ptr)
{
	t_gc	*gc;
	t_gc	*prev;

	gc = db->gc;
	prev = NULL;
	while (gc)
	{
		if (gc->ptr == ptr)
		{
			if (prev)
				prev->next = gc->next;
			else
				db->gc = gc->next;
			free(gc->ptr);
			free(gc);
			return ;
		}
		prev = gc;
		gc = gc->next;
	}
}
