#include "includes/main.h"

void    ascii_print()
{
    // print Lowashell
    printf("--------------------------------------------\n");
    printf(ORANGE"█░░ █▀█ █░█░█ ▄▀█ █▀ █░█ █▀▀ █░░ █░░"RESET"\n");
    printf(MAGENTA"█░▄ █▀▄ ▀▄▀▄▀ █▀█ ▄█ █▀█ █▄▄ █▄▄ █▄▄"RESET"\n");
    printf("\n");
    printf("All the rights are reserved to Lowashell :)\n");
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

    ascii_print();
    line = readline(MAGENTA"@lowa> "RESET);
    while (line)
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
        line = readline(MAGENTA"@lowa> "RESET);
    }
    printf("exit\n");
    free(line);
    return 0;
}
