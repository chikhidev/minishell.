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
int     strongest_operator(char *line, t_tracker *tracker);
int     count_between_op(t_db *db,  char *line, int op, t_tracker *tracker);
char    *remove_paranthesis(t_db *db, char *line, t_parnth *local_paranths);

/*string3*/
int     count_line_args(char *line, t_quote *quotes, int len);
int     append_split(char **splitted, char *string);
char    *sub(t_db *db, char *line, int i, int j);
int     all_whitespaces(char *line, int start, int end);
BOOL    is_the_first(char *line, t_tracker *tracker, int op);

/*parsing/quoting.c and parsing/quoting_utils.c*/
int     track_quotes(t_db *db, t_quote **head, char *line);
int     is_inside_quotes(t_quote *head, int i);
void    reset_quotes(t_db *db, t_quote **head);
int     inside_single_quote(t_quote *head, int i);

/*parsing/paranths.c && paranths2.c*/
t_parnth    *last_unclosed_paranth(t_parnth *head);
int is_operator_forward(char    *s, int  i);

/*parsing/parser.c*/
int parser(t_db *db, char *line);

/*parsing/split.c*/
int smart_split(t_db *db, char *line, void **current_node, void *parent);

/*parsing/expanding.c*/
int expand(t_db *db, char **line, t_quote *quotes);
char *get_env(t_db *db, char *name);
int valid_char(char c, int  index);

/*parsing/paranths.c and paranths2.c*/
int track_paranthesis(t_db *db, t_parnth **head, char *line, t_quote *quotes);
int is_inside_paranthesis(t_parnth *head, int i);

/*op.c*/
int is_valid_op(char c, char next_c);
int track_operators(t_db *db, char  *line);
int is_operator_backward(char    *s, int  i);
int check_after_op( char    *line,   char    *op_name,   int op_idx,  int flag);

/*op2.c*/
int is_op(char *line, int *i);
int priority_of_op(int op);
int create_op_node(t_db *db, int op, void **current_node, void *parent);

/*cmd.c*/
int     create_cmd_node(t_db *db, void **current_node, void *parent);
int     count_op(char *line, int op);

/*customized*/
char	**ft_new_split(t_db *db, t_quote *quotes, char *line);

/*expanding*/
char    *get_environment_var(char   *var, char *env[]);
/*nodes.c*/
// int     io_system(t_db *db, char *line, t_redirection **res, t_tracker *tracker);
// int     io_system(t_db *db, char **args, t_redirection **res);
int     validate_io(char *arg, int size);
int     open_heredoc(t_db *db, char *delim);
int     syntax_checker(t_db *db, char *line, int *start);


int     open_file(t_db *db, char *file, int type, t_quote *quotes);
int     count_array_len(char    **args);
bool    contains(char  *str, char    *sub);
int     find_c(char *str, char  c);
int     length_til(char *str, char stop);

#endif
