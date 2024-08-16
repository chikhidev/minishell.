#include "main.h"
#include "parsing.h"

void catch_feedback(t_db *db, int process_res)
{
    if (WIFEXITED(process_res))
    {
        db->last_signal = WEXITSTATUS(process_res);
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
    if (signal == SIGINT)
    {
        dprintf(2, "\n");
        exit(130);
    }
    if (signal == SIGQUIT)
    {
        dprintf(2, "^\\Quit (core dumped)\n");
        exit(131);
    }
    exit(1);
}

void cmd_signals_handling(void)
{
    struct sigaction sa;

    ft_bzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = cmd_behave;
    sigaction(SIGINT, &sa, NULL);
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

void parent_signals_handling(void)
{
    struct sigaction sa;

    ft_bzero(&sa, sizeof(struct sigaction));
    sa.sa_handler = SIG_IGN;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

