#include "main.h"
#include "parsing.h"

void catch_feedback(t_db *db, int process_res)
{
    int status;

    status = WEXITSTATUS(process_res);
    if (status)
    {
        db->last_signal = status;
        if (db->last_signal == FAIL)
        {
            gc_void(db);
            ec_void(db);
            exit(FAIL);
        }
    }
}

void heredoc_behave(int signal)
{
    if (signal == SIGINT)
    {
        write(2, "\n", 1);
        exit(130);
    }
    exit(1);
}

void default_signals_behav()
{
    struct sigaction sa;

    ft_bzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_DFL;
    sigaction(SIGQUIT, &sa, NULL);
}

void heredoc_signals_handling(void)
{
    struct sigaction sa;

    ft_bzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = heredoc_behave;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void parent_behav(int signal)
{
    if (signal == SIGINT)
    {
        write(2, "\n", 2);
    }
    if (signal == SIGQUIT)
    {
        write(2, "Quit (core dumped)\n", 20);
    }
}

void handle_parent_signals(void)
{
    struct sigaction sa;

    ft_bzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = parent_behav;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

