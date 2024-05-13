#include "includes/main.h"

int main(void)
{
    char    *line;
    t_root  *root;

    root = NULL;
    line = readline("> ");
    while (line)
    {
        add_history(line);
        parse(line, root);
        free(line);
        line = readline("> ");
    }
    printf("exit\n");
    free_memo(root);
    return 0;
}
