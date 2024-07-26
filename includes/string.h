#include "main.h"

#define SGL_QUOTE '\''
#define DBL_QUOTE '\"'

char    *concat(t_db *db, char *s, char single_char);
void skip_spaces(char *line, int *i);
int count(char *line, char c);
int are_all(char *str, int (*f)(int));
int all_whitespaces(char *line, int start, int end);
char *remove_paranthesis(t_db *db, char *line, t_parnth *local_paranths);
int is_whitespace(char c);
int strongest_operator(char *line, t_tracker *tracker);
int count_between_op(t_db *db,  char *line, int op, t_tracker *tracker);
int count_array_len(char    **args);
int count_line_args(char *line, t_quote *quotes, int len);
int append_split(char **splitted, char *string);
char *sub(t_db *db, char *line, int i, int j);
char *whithout_quotes_free_db(t_db *db, char *line);
bool contains(char  *str, char    *sub);
int find_c(char *str, char  c);
int length_til(char *str, char stop);
int ft_strcmp(const char *s1, const char *s2);
char	*ft_strjoin_char(char *s1, char c);
char *whithout_quotes( char *line);