#include "includes/main.h"

void    ascii_print()
{
    printf("\n\n");
    printf(ORANGE"█░░ █▀█ █░█░█ ▄▀█ █▀ █░█ █▀▀ █░░ █░░"RESET"\n");
    printf(MAGENTA"█░▄ █▀▄ ▀▄▀▄▀ █▀█ ▄█ █▀█ █▄▄ █▄▄ █▄▄"RESET"\n");
    printf("\n");
    printf("All the rights are reserved to Lowashell 🚀:)\n");
    printf("--------------------------------------------\n\n");
}

void    init_root(t_root *root)
{
    root->error = 0;
    root->buffer = NULL;
    root->tmp = NULL;
    root->quotes = NULL;
    root->putted = 0;
    root->num_commands = -1;
    root->cmds = NULL;
    root->commands = NULL;
}

int main(void)
{
    char    *line;
    t_root  root;
    char    *userHost;
    char    *tmp;
    
    ascii_print();
    userHost = getenv("USER");
    tmp = ft_strjoin(MAGENTA"lowa@"GREEN, userHost);
    if (!tmp)
        return 1;
    userHost = ft_strjoin(tmp, ": "RESET);
    if (!userHost)
        return 1;
    free(tmp);
    line = readline(userHost);
    if (!line)
        line = ft_strdup("");
    while (ft_strncmp(line, "exit", 4) != 0)
    {
        init_root(&root);
        if (root.error)
            root.error = 0;
        add_history(line);
        parse(line, &root);
        // printf("before generate\n");
        free(line);
        free_memo(&root);
        // printf("after parse\n");
        line = readline(userHost);
        if (!line)
            line = ft_strdup("");
    }
    free(userHost);
    printf("_____Goodbye! 👋_____\n"RESET);
    free(line);
    return 0;
}
