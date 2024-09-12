/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:21 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/12 08:38:36 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

int	ft_open(t_db *db, char *file, int flags, int type)
{
	int	fd;

	if (type == -1)
		fd = open(file, flags);
	else
		fd = open(file, flags, type);
	fd_add(db, fd);
	return (fd);
}

int	create_redirection(t_db *db, int type, int fd)
{
	if (type == INPUTFILE)
	{
		if (db->input_fd != STDIN_FILENO && db->input_fd != INVALID)
		{
			close(db->input_fd);
		}
		db->input_fd = fd;
		db->curr_type = INPUTFILE;
	}
	else if (type == APPENDFILE || type == OUTPUTFILE)
	{
		if (db->output_fd != STDOUT_FILENO && db->output_fd != INVALID)
		{
			close(db->output_fd);
		}
		db->output_fd = fd;
		db->curr_type = type;
	}
	return (SUCCESS);
}

int	check_ambigious(t_db *db, char *file)
{
	t_exp_list	*store;

	if (file[0] == '$' && file[1] != '\0')
	{
		store = get_exp_node(db->exp_list, file + 1);
		if (!store || (store->val && store->val[0] == '\0'))
		{
			(put_fd(2, file), put_fd(2, " :ambiguous redirect\n"));
			return (true);
		}
		if (contains_spaces_btwn(store->val))
		{
			(put_fd(2, file), put_fd(2, " :ambiguous redirect\n"));
			return (true);
		}
		if (count_array_len(ft_split(db, store->val, " \t\n\r\v\f")) > 1)
		{
			(put_fd(2, file), put_fd(2, " :ambiguous redirect\n"));
			return (true);
		}
	}
	return (false);
}

int	open_file_p2(t_db *db, char *tmp, int type)
{
	int	fd;

	fd = INVALID;
	if (ft_strlen(tmp) == 0)
	{
		put_fd(2, "No such file or directory\n");
		return (FAILURE);
	}
	if (db->split)
		tmp = ft_split(db, tmp, " \t\n\r\v\f")[0];
	if (type == APPENDFILE)
		fd = ft_open(db, tmp, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (type == INPUTFILE)
		fd = ft_open(db, tmp, O_RDONLY, -1);
	else if (type == OUTPUTFILE)
		fd = ft_open(db, tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (create_redirection(db, type, fd) == FAILURE)
		return (FAILURE);
	if (fd == INVALID)
	{
		perror(tmp);
		return (FAILURE);
	}
	return (SUCCESS);
}

int	open_file(t_db *db, char *file, int type)
{
	t_quote	*quotes;
	char	*tmp;

	quotes = NULL;
	if (!file || ft_strlen(file) == 0)
		return (SUCCESS);
	if (check_ambigious(db, file) == true)
	{
		create_redirection(db, type, INVALID);
		return (FAILURE);
	}
	if (track_quotes(db, &quotes, file) == FAILURE)
		return (FAILURE);
	if (expand(db, &file, &quotes) == FAILURE)
		return (FAILURE);
	tmp = without_quotes(db, file, quotes);
	if (open_file_p2(db, tmp, type) == FAILURE)
		return (FAILURE);
	return (SUCCESS);
}
