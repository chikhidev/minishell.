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
 * @details Defining the representations of the operators:
*/
#define OR 1
#define AND 2
#define PIPE 3
#define REDIR 4
#define APPEND 5
#define INPUT 6
#define HEREDOC 7

/*string.c*/
void    skip_spaces(char *line, int *i);
int     are_all(char *str, int (*f)(int));

/*string2.c*/
int     is_whitespace(char c);
int     strongest_operator(t_db *db, t_parnth *head, char *line);
int     count_between_op(t_db *db, t_parnth *head, char *line, int op);
char    *remove_paranthesis(t_db *db, char *line, t_parnth *local_paranths);

/*parsing/quoting.c and parsing/quoting_utils.c*/
int     track_quotes(t_db *db, char *line);
int     is_inside_quotes(t_db *db, int i);
void    reset_quotes(t_db *db);
int     inside_single_quote(t_db *db, int   i);

/*parsing/paranths.c && paranths2.c*/
t_parnth    *last_unclosed_paranth(t_parnth *head);
int is_operator2(char    *s, int  i);

/*parsing/parser.c*/
int parser(t_db *db, char *line);

/*parsing/split.c*/
int smart_split(t_db *db, char *line, void **current_node, void *parent);

/*parsing/expanding.c*/
int expand(t_db *db, char **line);
char *get_env(t_db *db, char *name);
int valid_char(char c, int  index);

/*parsing/paranths.c and paranths2.c*/
int track_paranthesis(t_db *db, t_parnth **head, char *line);
int is_inside_paranthesis(t_parnth *head, int i);

/*op.c*/
int is_valid_op(char c, char next_c);
int track_operators(t_db *db, char  *line);
int is_operator(char    *s, int  i);
int check_after_op( char    *line,   char    *op_name,   int op_idx,  int flag);

/*op2.c*/
int is_op(char *line, int *i);
int priority_of_op(int op);
int create_op_node(t_db *db, int op, void **current_node, void *parent);

#endif
