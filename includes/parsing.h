#ifndef PARSING_H
#define PARSING_H

#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>


/**
 * @details Defining the priority of the operators
*/
#define AND_OR_PRIORITY 1
#define PIPE_PRIORITY 2
#define REDIR_PRIORITY 3

/**
 * @details Defining the type of the operators
*/
#define OR 1
#define AND 2
#define PIPE 3
#define REDIR 4
#define APPEND 5
#define INPUT 6

/*parsing/parser.c*/
int parser(t_db *db, char *line);

/*string.c*/
void    skip_spaces(char *line, int *i);

/*parsing/quoting.c*/
int track_quotes(t_db *db, char *line);

/*parsing/paranths.c && paranths2.c*/
int         track_paranthesis(t_db *db, char *line);
t_parnth    *last_unclosed_paranth(t_db *db);

#endif