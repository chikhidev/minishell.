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

void default_signals_behav(bool ignore_quit)
{
    signal(SIGINT, SIG_DFL);
    if (!ignore_quit)
        signal(SIGQUIT, SIG_DFL);
    else
        signal(SIGQUIT, SIG_IGN);
}

void heredoc_signals_handling(void)
{
    signal(SIGINT, heredoc_behave);
    signal(SIGQUIT, SIG_IGN);
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
    signal(SIGINT, parent_behav);
    signal(SIGQUIT, parent_behav);
}

