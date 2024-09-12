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

void	default_signals_behav(bool ignore_quit)
{
	signal(SIGINT, SIG_DFL);
	if (!ignore_quit)
		signal(SIGQUIT, SIG_DFL);
	else
		signal(SIGQUIT, SIG_IGN);
}

void	parent_behav(int signal)
{
	t_db *db;

	db = this();
	if (signal == SIGINT)
		ft_write(db, 2, "\n", 2);
	if (signal == SIGQUIT)
		ft_write(db, 2, "Quit\n", 5);
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

//void	interactive_sigint(int sig)
//{
//	(void)sig;
//	t_db	*db;
//	printf("\n");
//	rl_on_new_line();
//	rl_replace_line("", 0);
//	rl_redisplay();
//	set_exit_status(*((get_list_collecter())->env), 130);
//	db->last_status = 130 >> 8;
//}

//void	interactive_sigquit(int sig)
//{
//	(void)sig;
//	return ;
//}

//void	active_sigint(int sig)
//{
//	(void)sig;
//	printf("\n");
//	free_at_exit();
//	exit(130);
//}

//void	active_sigquit(int sig)
//{
//	(void)sig;
//	printf("Quit (core dumped)\n");
//}

//void	handle_signals(void (*sigint)(int), void (*sigquit)(int),
//		void (*sigint_old)(int), void (*sigquit_old)(int))
//{
//	struct sigaction	s_int;
//	struct sigaction	s_int_old;
//	struct sigaction	s_quit;
//	struct sigaction	s_quit_old;

//	s_int.sa_handler = sigint;
//	sigemptyset(&s_int.sa_mask);
//	s_int.sa_flags = 0;

//	s_int_old.sa_handler = sigint_old;
//	sigemptyset(&s_int_old.sa_mask);
//	s_int_old.sa_flags = 0;
//	sigaction(SIGINT, &s_int, &s_int_old);

//	s_quit.sa_handler = sigquit;
//	sigemptyset(&s_quit.sa_mask);
//	s_quit.sa_flags = 0;

//	s_quit_old.sa_handler = sigquit_old;
//	sigemptyset(&s_quit_old.sa_mask);
//	s_quit_old.sa_flags = 0;
//	sigaction(SIGQUIT, &s_quit, &s_quit_old);
//}