/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pid.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:48:12 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 21:00:13 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"

void	*pid_add(t_db *db, pid_t new_ip)
{
	t_int	*ip;

	if (!db->pid)
	{
		db->pid = gc_malloc(db, sizeof(t_int));
		if (!db->pid)
			return (NULL);
		db->pid->n = new_ip;
		db->pid->next = NULL;
	}
	else
	{
		ip = db->pid;
		while (ip->next)
			ip = ip->next;
		ip->next = gc_malloc(db, sizeof(t_int));
		if (!ip->next)
			return (NULL);
		ip->next->n = new_ip;
		ip->next->next = NULL;
	}
	return (NULL);
}

void	pid_free(t_db *db, pid_t ip_to_free)
{
	t_int	*ip;
	t_int	*prev;

	ip = db->pid;
	prev = NULL;
	while (ip)
	{
		if (ip->n == ip_to_free)
		{
			if (prev)
				prev->next = ip->next;
			else
				db->pid = ip->next;
			gc_free(db, ip);
			return ;
		}
		prev = ip;
		ip = ip->next;
	}
}

void	pid_void(t_db *db)
{
	t_int	*ip;

	while (db->pid)
	{
		ip = db->pid;
		db->pid = db->pid->next;
		gc_free(db, ip);
	}
	db->pid = NULL;
}
