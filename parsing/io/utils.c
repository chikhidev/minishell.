/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 07:06:53 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/16 20:45:06 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

int	validate_io(char *arg, int size)
{
	if (ft_strncmp(arg, ">>", 2) == 0 && size == 2)
		return (APPENDFILE);
	if (ft_strncmp(arg, "<<", 2) == 0 && size == 2)
		return (HEREDOC);
	if (ft_strncmp(arg, ">", 1) == 0 && size == 1)
		return (OUTPUTFILE);
	if (ft_strncmp(arg, "<", 1) == 0 && size == 1)
		return (INPUTFILE);
	return (INVALID);
}

int	read_and_fill(t_db *db, char *line, int *pipe_fd, char *delim)
{
	char	*tmp;

	line = readline("> ");
	if (!line)
	{
		put_fd(2, "Warning: here-document delimited by end-of-file\n");
		ft_close(db, &pipe_fd[1]);
		return (FAILURE);
	}
	tmp = ft_strdup(db, line);
	free(line);
	if (ft_strcmp(delim, tmp) == 0 || ((int)len(delim) == 0
			&& (int)len(tmp) == 0))
		return (ft_close(db, &pipe_fd[1]), FAILURE);
	if (db->expand_hrdc == 0 && expand(&tmp, NULL) == FAILURE)
	{
		ft_close(db, &pipe_fd[1]);
		ec_void(db);
		exit(1);
	}
	ft_write(db, pipe_fd[1], tmp, (int)len(tmp));
	ft_write(db, pipe_fd[1], "\n", 1);
	return (SUCCESS);
}

void	heredoc_child(char *delim, int *pipe_fd)
{
	t_db	*db;
	char	*line;

	db = this();
	line = NULL;
	handle_here_doc_signals();
	ft_close(db, &pipe_fd[0]);
	while (1)
	{
		if (read_and_fill(db, line, pipe_fd, delim) == FAILURE)
			break ;
	}
	if (db->input_fd != STDIN_FILENO && db->input_fd != INVALID)
		ft_close(db, &db->input_fd);
	ft_exit(db, 0, 3, NULL);
}

void	expand_herdoc(t_quote *q, char **delim)
{
	this()->expand_hrdc = (int)len(*delim);
	*delim = without_quotes(this(), *delim, q);
	this()->expand_hrdc -= (int)len(*delim);
}

int	open_heredoc(t_db *db, char *delim)
{
	int		*pipe_fd;
	int		pid;
	int		child_status;
	t_quote	*q;

	q = NULL;
	if (track_quotes(db, &q, delim) == FAILURE)
		return (FAILURE);
	expand_herdoc(q, &delim);
	pipe_fd = gc_malloc(db, sizeof(int) * 2);
	ft_pipe(db, pipe_fd);
	pid = fork();
	if (pid == -1)
		return (error(db, "fork", NULL));
	if (pid == CHILD)
		heredoc_child(delim, pipe_fd);
	ft_close(db, &pipe_fd[1]);
	wait(&child_status);
	catch_feedback(db, child_status);
	if (db->last_status != 0)
		return (FAILURE);
	db->input_fd = pipe_fd[0];
	db->curr_type = HEREDOC;
	return (SUCCESS);
}
