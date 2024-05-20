#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <readline/history.h>

#include "../libft/libft.h"

#define RED     "\033[0;31m"
#define MAGENTA "\033[0;35m"
#define ORANGE  "\033[0;33m"
#define GREEN "\033[0;92m"
#define RESET   "\033[0m"

typedef struct s_command
{
    char *cmd;
    char **args;
}   t_command;

typedef struct s_quote
{
    int     ascii;
    int     start;
    int     end;
}   t_quote;

typedef struct s_root
{
    int         error;
    char        *buffer;
    char        *tmp;
    t_quote     *quotes;
    int         putted;
    int         num_commands;
    char        **cmds;
    t_command   **commands;
}   t_root;

/*prototypes*/

/*parse*/
void    parse(char *line, t_root *root);

/*error*/
void    error(char *msg, t_root *root);

/*memo*/
void    free_memo(t_root *root);
void    *ft_realloc(void *ptr, size_t size);

/*quotes*/
void    quotes_tracking(t_root *root, char _character_, int i, int *quotes_count);
void    quotes_checker(t_root *root, int quotes_count, void (f)(char *, t_root *, int));

/*string*/
char    *concat_str(t_root *root, char *buffer, char _char_);
int     is_white_space(char c);

#endif