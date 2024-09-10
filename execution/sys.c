#include "main.h"
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