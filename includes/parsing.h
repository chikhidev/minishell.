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

#define OPEN_PARANTH '('
#define CLOSE_PARANTH ')'

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

#define W_SPACE 8

typedef struct s_holder
{
    int op;
    void **current_node;
    void *parent;

    t_tracker *tracker;
} t_holder;

/*string.c*/
void skip_spaces(char *line, int *i);
int are_all(char *str, int (*f)(int));

/*string2.c*/
int is_whitespace(char c);
int strongest_operator(char *line, t_tracker *tracker);
int count_between_op(t_db *db, char *line, int op, t_tracker *tracker);

/*string3*/
int count_line_args(char *line, t_quote *quotes, int len);
int append_split(char **splitted, char *string);
char *sub(t_db *db, char *line, int i, int j);
int all_whitespaces(char *line, int start, int end);
int is_special(t_db *db, char *s, int *i, t_quote *quotes);

/*parsing/quoting.c and parsing/quoting_utils.c*/
int track_quotes(t_db *db, t_quote **head, char *line);
bool is_inside_quotes_list(t_quote *head, int i);
void update_quotes(t_quote *head, int start, int old_len, int new_len);
bool inside_single_quote(t_quote *head, int i);

/*parsing/paranths.c && paranths2.c*/
int is_operator_forward(char *s, int i);

/*parsing/parser.c*/
int parser(t_db *db, char *line);

/*parsing/split.c*/
int smart_split(t_db *db, char *line, void **current_node, void *parent);

/*parsing/expanding.c*/
int expand(t_db *db, char **line, t_quote **quotes);
char *get_env(t_db *db, char *name);
int valid_char(char c, int index);

/*op.c*/
int is_valid_op(char c, char next_c);
int track_operators(t_db *db, char *line, t_quote *quotes);
int is_operator_backward(char *s, int i);
int check_after_op(char *line, char *op_name, int op_idx, int flag);

/*op2.c*/
int is_op(char *line, int *i);
int priority_of_op(int op);
int create_op_node(t_db *db, int op, void **current_node);

/*cmd.c*/
int create_cmd_node(t_db *db, void **current_node);
int count_op(char *line, int op);

/*customized*/
char **tokenize(t_db *db, t_quote **quotes, char *s);

/*expanding*/
char *get_environment_var(t_db *db, char *var, char *env[]);

/*nodes.c*/
int validate_io(char *arg, int size);
int open_heredoc(t_db *db, char *delim);
int syntax_checker(t_db *db, char *line, t_quote *quotes);
int open_file(t_db *db, char *file, int type);
int count_array_len(char **args);
bool contains(char *str, char *sub);
int find_c(char *str, char c);
int length_til(char *str, char stop);
bool is_newline_at_the_end(char *line);
char *gc_copy(t_db *db, char *s);

/*quoting*/
void skip_open_spaces(t_quote *quotes, char *line, int *i);
bool is_quote_oppening(t_quote *head, int i);

char **append_word(t_db *db, char **result, char *save);
int handle_wildcard(t_db *db, char **result);

t_file_entry *new_ent_node(t_db *db, struct dirent *ent);
void add_ent_front(t_file_entry **list, t_file_entry *new);
void push_ent_back(t_file_entry **list, t_file_entry *new);
t_file_entry *get_ent_node(t_file_entry *list, char *name, unsigned char type);
void del_ent_node(t_file_entry **list, char *name, unsigned char type);

t_str_lst *new_str_node(t_db *db, char *str);
void add_str_front(t_str_lst **list, t_str_lst *new);
void push_str_back(t_str_lst **list, t_str_lst *new);
t_str_lst *get_str_node(t_str_lst *list, char *str);
void del_str_node(t_str_lst **list, char *str);

#endif
