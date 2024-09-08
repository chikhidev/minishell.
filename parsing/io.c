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
		if (!store)
		{
			printf("%s: ambiguous redirect\n", file);
			return (true);
		}
		if (store->val && store->val[0] == '\0')
		{
			printf("%s: ambiguous redirect\n", file);
			return (true);
		}
		if (contains_spaces_btwn(store->val))
		{
			printf("%s: ambiguous redirect\n", file);
			return (true);
		}
		if (count_array_len(
				ft_split(db, store->val, " \t\n\r\v\f")) > 1)
		{
			printf("%s: ambiguous redirect\n", file);
			return (true);
		}
	}
	return (false);
}

int	open_file(t_db *db, char *file, int type)
{
	t_quote	*quotes;
	int		fd;
	char	*tmp;

	quotes = NULL;
	if (!file || ft_strlen(file) == 0)
		return (SUCCESS);
	fd = INVALID;
	if (check_ambigious(db, file) == true)
	{
		create_redirection(db, type, INVALID);
		return (FAILURE);
	}
	// printf("file: %s|\n", file);
	// tmp = whithout_quotes(db, file);
	// printf("tmp: %s|\n", tmp);
	if (track_quotes(db, &quotes, file) == FAILURE)
		return (FAILURE);
	if (expand(db, &file, &quotes) == FAILURE)
		return (FAILURE);
	tmp = without_quotes(db, file, quotes);
	if (ft_strlen(tmp) == 0)
	{
		printf("No such file or directory\n");
		return FAILURE;
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

int	open_heredoc(t_db *db, char *delim)
{
	int		*pipe_fd;
	int		pid;
	char	*line;
	char	*tmp;
	int		child_status;
	t_quote	*q;

	q = NULL;
	if (track_quotes(db, &q, delim) == FAILURE)
		return (FAILURE);
	tmp = without_quotes(db, delim, q);
	delim = tmp;
	pipe_fd = gc_malloc(db, sizeof(int) * 2);
	ft_pipe(db, pipe_fd);
	pid = fork(); /* we fork to handle signals inside the child */
	if (pid == -1)
		return (error(db, "fork", NULL));
	if (IS_CHILD)
	{
		// default_signals_behav(true);
		handle_here_doc_signals();
		ft_close(db, &pipe_fd[0]);
		while (1)
		{
			line = readline("> ");
			if (!line)
			{
				printf("Warning: here-document delimited by end-of-file\n");
				ft_close(db, &pipe_fd[1]);
				break ;
			}
			tmp = ft_strdup(db, line);
			free(line);
			if (ft_strcmp(delim, tmp) == 0)
			{
				ft_close(db, &pipe_fd[1]);
				break ;
			}
			if (expand(db, &tmp, NULL) == FAILURE)
			{
			    ft_close(db, &pipe_fd[1]);
			    ec_void(db);
			    exit(1);
			}
			// printf("tmp: %s\n", tmp);
			write(pipe_fd[1], tmp, ft_strlen(tmp));
			write(pipe_fd[1], "\n", 1);
		}
		if (db->input_fd != STDIN_FILENO && db->input_fd != INVALID)
		{
			ft_close(db, &db->input_fd);
		}
		gc_void(db);
		ec_void(db);
		exit(0); /* Exit normally */
	}
	/*-------------------------------Parent process------------------------------*/
	// cancel SIGINT and SIGQUIT they sound be handled by the child
	ft_close(db, &pipe_fd[1]);
	wait(&child_status);
	catch_feedback(db, child_status);
	if (db->last_status != 0)
		return (FAILURE);
	db->input_fd = pipe_fd[0];
	db->curr_type = HEREDOC;
	return (SUCCESS);
}
