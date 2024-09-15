/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/13 16:57:14 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:55:04 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/main.h"
#include "includes/exec.h"

int	error(t_db *db, char *specifier, char *msg)
{
	if (msg)
	{
		if (!specifier)
		{
			ft_write(db, 2, msg, (int)len(msg));
			ft_write(db, 2, "\n", 1);
		}
		else
		{
			ft_write(db, 2, specifier, (int)len(specifier));
			ft_write(db, 2, ": ", 2);
			ft_write(db, 2, msg, (int)len(msg));
			ft_write(db, 2, "\n", 1);
		}
	}
	fd_void(db);
	gc_void(db);
	db->error = true;
	if (db->last_status == 0)
		db->last_status = 2;
	return (FAILURE);
}
