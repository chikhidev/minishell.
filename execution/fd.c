/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:59:55 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 21:00:09 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

void	*fd_add(t_db *db, pid_t new_fd)
{
	t_int	*ip;

	if (!db->fd)
	{
		db->fd = gc_malloc(db, sizeof(t_int));
		db->fd->n = new_fd;
		db->fd->next = NULL;
	}
	else
	{
		ip = db->fd;
		while (ip->next)
			ip = ip->next;
		ip->next = gc_malloc(db, sizeof(t_int));
		ip->next->n = new_fd;
		ip->next->next = NULL;
	}
	return (NULL);
}

void	fd_free(t_db *db, pid_t fd_to_free)
{
	t_int	*fd;
	t_int	*prev;

	fd = db->fd;
	prev = NULL;
	while (fd)
	{
		if (fd->n == fd_to_free)
		{
			if (prev)
				prev->next = fd->next;
			else
				db->fd = fd->next;
			gc_free(db, fd);
			return ;
		}
		prev = fd;
		fd = fd->next;
	}
}

void	fd_void(t_db *db)
{
	t_int	*tmp;

	while (db->fd)
	{
		tmp = db->fd->next;
		if (db->fd->n > 2)
			close(db->fd->n);
		gc_free(db, db->fd);
		db->fd = tmp;
	}
	db->fd = NULL;
}
