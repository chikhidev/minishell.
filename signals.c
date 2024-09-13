#include "main.h"
#include "parsing.h"

void	catch_feedback(t_db *db, int process_res)
{
	int	status;

	status = WEXITSTATUS(process_res);
	db->last_status = status;
	if (status && db->last_status == FAIL)
	{
		gc_void(db);
		ec_void(db);
		exit(FAIL);
	}
}

void	default_signals_behav(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	parent_behav(int signal)
{
	t_db *db;

	db = this();
	if (signal == SIGINT)
	{
		ft_write(db, 2, "\n", 2);
	}
	if (signal == SIGQUIT)
	{
		ft_write(db, 2, "Quit\n", 5);
	}
}

void	handle_parent_signals(void)
{
	signal(SIGINT, parent_behav);
	signal(SIGQUIT, parent_behav);
}

void	heredoc_behave(int signal)
{
	t_db	*db;

	db = this();
	if (signal == SIGINT)
	{
		ft_write(db, 2, "\n", 2);
		gc_void(db);
		ec_void(db);
		exit(130);
	}
}

void	handle_here_doc_signals(void)
{
	signal(SIGINT, heredoc_behave);
	signal(SIGQUIT, SIG_IGN);
}
