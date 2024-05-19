#include "includes/main.h"

void    error(char *msg, t_root *root)
{
    printf("Error: %s\n", msg);
    free_memo(root);
    root->error = 1;
}