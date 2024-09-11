/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sys.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:48:16 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 20:51:10 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "exec.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	ft_exit(t_db *db, int status, short free_flag, char *msg)
{
	if (msg)
		put_fd(2, msg);
	if (msg)
		put_fd(2, "\n");
	if (free_flag > 2)
		fd_void(db);
	if (free_flag > 1)
		ec_void(db);
	if (free_flag > 0)
		gc_void(db);
	exit(status);
}

int	ft_write(t_db *db, int fd, char *msg, int len)
{
	int	res;

	res = write(fd, msg, len);
	if (res == -1)
		ft_exit(db, 1, 3, "write failed");
	return (res);
}

void	ft_close(t_db *db, int *fd)
{
	int	res;

	res = 0;
	if (*fd != CLOSED)
	{
		res = close(*fd);
		fd_free(db, *fd);
	}
	if (res != INVALID)
		return ;
	ft_exit(db, 1, 3, "close failed");
}

void	ft_pipe(t_db *db, int *pipe_fd)
{
	int	res;

	res = pipe(pipe_fd);
	if (res != INVALID && pipe_fd[0] != INVALID)
		fd_add(db, pipe_fd[0]);
	if (res != INVALID && pipe_fd[1] != INVALID)
		fd_add(db, pipe_fd[1]);
	if (res != INVALID)
		return ;
	ft_exit(db, 1, 3, "pipe failed");
}
