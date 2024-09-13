/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 02:24:31 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/13 16:55:42 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/main.h"
#include "includes/string.h"

void	add_front_gc(t_db *db, void *ptr)
{
	db->gc = malloc(sizeof(t_gc));
	if (!db->gc)
		(free(ptr), ec_void(db), exit(FAIL));
	db->gc->ptr = ptr;
	db->gc->next = NULL;
}

void	add_front_ec(t_db *db, void *ptr)
{
	db->ec = malloc(sizeof(t_gc));
	if (!db->ec)
		(free(ptr), ec_void(db), exit(FAIL));
	db->ec->ptr = ptr;
	db->ec->next = NULL;
}

void	*gc_malloc(t_db *db, size_t size)
{
	void	*ptr;
	t_gc	*gc;

	ptr = malloc(size + 1);
	if (!ptr)
		(gc_void(db), ec_void(db), exit(FAIL));
	ft_bzero(ptr, size + 1);
	if (!db->gc)
		add_front_gc(db, ptr);
	else
	{
		gc = db->gc;
		while (gc->next)
			gc = gc->next;
		gc->next = malloc(sizeof(t_gc));
		if (!gc->next)
		{
			gc->next = NULL;
			(free(ptr), gc_void(db), ec_void(db), exit(FAIL));
		}
		gc->next->ptr = ptr;
		gc->next->next = NULL;
	}
	return (ptr);
}

void	*ec_malloc(t_db *db, size_t size)
{
	void	*ptr;
	t_gc	*ec;

	ptr = malloc(size);
	if (!ptr)
		(gc_void(db), ec_void(db), exit(FAIL));
	if (!db->ec)
		add_front_ec(db, ptr);
	else
	{
		ec = db->ec;
		while (ec->next)
			ec = ec->next;
		ec->next = malloc(sizeof(t_gc));
		if (!ec->next)
		{
			ec->next = NULL;
			free(ptr);
			gc_void(db);
			exit(FAIL);
		}
		ec->next->ptr = ptr;
		ec->next->next = NULL;
	}
	return (ptr);
}
