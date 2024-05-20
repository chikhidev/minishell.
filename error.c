#include "includes/main.h"

void    error(char *msg, t_root *root)
{
    printf(RED"Error: %s\n"RESET, msg);
    free_memo(root);
    root->error = 1;
    printf("exit error function\n");
}