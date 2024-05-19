#include "includes/main.h"

int main(void)
{
    char    *line;
    t_root  *root;

    root = NULL;
    line = readline(YELLOW"> "RESET);
    while (line)
    {
        if (root && root->error)
            root->error = 0;
        add_history(line);
        parse(line, root);
        free(line);
        line = readline(YELLOW"> "RESET);
    }
    printf("exit\n");
    free_memo(root);
    free(line);
    free(root);
    return 0;
}
