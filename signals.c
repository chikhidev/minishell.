#include "main.h"
#include "parsing.h"

void catch_feedback(t_db *db, int process_res)
{
    int status;

    status = WEXITSTATUS(process_res);
    db->last_status = status;
    if (status)
    {
        printf("exit status: %d\n", status);
        if (db->last_status == FAIL)
        {
            gc_void(db);
            ec_void(db);
            exit(FAIL);
        }
    }
}

void default_signals_behav(bool ignore_quit)
{
    signal(SIGINT, SIG_DFL);
    if (!ignore_quit)
        signal(SIGQUIT, SIG_DFL);
    else
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
        write(2, "Quit\n", 5);
    }
}

void handle_parent_signals(void)
{
    signal(SIGINT, parent_behav);
    signal(SIGQUIT, parent_behav);
}

void heredoc_behave(int signal)
{
    t_db *db;

    db = this();
    if (signal == SIGINT)
    {
        write(2, "\n", 2);
        gc_void(db);
        ec_void(db);
        exit(130);
    }
}

void handle_here_doc_signals(void)
{
    signal(SIGINT, heredoc_behave);
    signal(SIGQUIT, SIG_IGN);
}
