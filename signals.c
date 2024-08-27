#include "main.h"
#include "parsing.h"

void catch_feedback(t_db *db, int process_res)
{
    if (WIFEXITED(process_res))
    {
        db->last_signal = WEXITSTATUS(process_res);
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

void cmd_behave(int signal)
{
    write(2, "i am here\n", 10);
    if (signal == SIGINT)
    {
        write(2, "\n", 2);
        exit(130);
    }
    if (signal == SIGQUIT)
    {
        write(2, "^\\Quit (core dumped)\n", 22);
        exit(131);
    }
    exit(1);
}

void cmd_signals_handling(void)
{
    struct sigaction sa;

    // dprintf(2, "cmd in progress %d\n", getpid());
    ft_bzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = cmd_behave;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = cmd_behave;
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

void ignore_signals(void)
{
    struct sigaction sa;

    // dprintf(2, "ignore in progress %d\n", getpid());
    ft_bzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}
