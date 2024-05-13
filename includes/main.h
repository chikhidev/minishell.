#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

typedef struct s_command
{
    char *cmd;
    char **args;
}   t_command;

typedef struct s_root
{
    int         num_commands;
    t_command   **commands;
}   t_root;

/*prototypes*/

/*parse*/
void    parse(char *line, t_root *root);

/*error*/
void    error(char *msg, t_root *root);

/*memo*/
void    free_memo(t_root *root);

#endif