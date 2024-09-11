/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys1.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:51:21 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 20:53:17 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	ft_dup2(t_db *db, int old_fd, int new_fd)
{
	int	res;

	res = dup2(old_fd, new_fd);
	if (old_fd != INVALID)
		fd_add(db, old_fd);
	if (res != INVALID)
		return ;
	ft_exit(db, 1, 3, "dup2 failed");
}

int	ft_dup(t_db *db, int fd)
{
	int	new_fd;

	new_fd = dup(fd);
	if (fd != INVALID)
		fd_add(db, new_fd);
	if (new_fd != INVALID)
		return (new_fd);
	ft_exit(db, 1, 3, "dup failed");
	return (new_fd);
}
