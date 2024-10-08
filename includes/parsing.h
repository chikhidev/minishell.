/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:16:31 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/16 20:44:02 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSING_H
# define PARSING_H

# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>

# define PIPE 3
# define REDIR 4
# define APPEND 5
# define INPUT 6
# define HEREDOC 7
# define W_SPACE 8
# define WORD 9

typedef struct s_holder
{
	int			op;
	void		**current_node;
	void		*parent;
	t_tracker	*tracker;

}				t_holder;

typedef struct s_syntax
{
	int			curr;
	int			i;
	bool		found_word;
}				t_syntax;

/*string.c*/
void			skip_spaces(char *line, int *i);
int				are_all(char *str, int (*f)(int));

/*string2.c*/
int				is_whitespace(char c);
int				count_between_op(t_db *db, char *line, int op,
					t_tracker *tracker);

/*string3*/
char			*sub(t_db *db, char *line, int i, int j);
int				all_whitespaces(char *line, int start, int end);
int				get_tok(t_db *db, char *s, int *i, t_quote *quotes);

bool			is_open_whitespace(char *line, int i, t_quote *quotes);
bool			is_open_io(char *line, int i, t_quote *quotes);

/*parsing/quoting.c and parsing/quoting_utils.c*/
int				track_quotes(t_db *db, t_quote **head, char *line);
bool			is_inside_quotes_list(t_quote *head, int i);
bool			inside_single_quote(t_quote *head, int i);
void			update_quotes(t_quote *head, int start, int old_len,
					int new_len);
void			delete_quotes_in_range(t_quote **head, int start, int end);
t_quote			*quote_at(t_quote *head, int index);
t_quote			*quotes_of_range(t_quote *head, int pos);
bool			after_quote_ending(t_quote *head, int i);

/*parsing/paranths.c && paranths2.c*/
int				is_operator_forward(char *s, int i);

/*parsing/parser.c*/
int				parser(t_db *db, char *line);

/*parsing/split.c*/
int				smart_split(t_db *db, char *line, void **current_node,
					void *parent);

/*parsing/expanding.c*/
int				expand(char **line, t_quote **quotes);
char			*get_env(t_db *db, char *name);
int				valid_char(char c, int index);

/*op.c*/
int				is_valid_op(char c, char next_c);
int				track_operators(t_db *db, char *line, t_quote *quotes);
int				is_operator_backward(char *s, int i);
int				check_after_op(char *line, char *op_name, int op_idx, int flag);

/*op2.c*/
int				is_op(char *line, int *i);
int				priority_of_op(int op);
int				create_op_node(t_db *db, int op, void **current_node);

/*cmd.c*/
int				create_cmd_node(t_db *db, void **curr);
int				count_op(char *line, int op);

/*customized*/
char			**tokenize(t_db *db, t_quote **quotes, char *s);

/*expanding*/
char			*get_environment_var(t_db *db, char *var, char *env[]);

/*nodes.c*/
int				validate_io(char *arg, int size);
int				open_heredoc(t_db *db, char *delim);
int				open_file(t_db *db, char *file, int type);
int				count_array_len(char **args);
bool			contains(char *str, char *sub);
int				find_c(char *str, char c);
int				length_til(char *str, char stop);
bool			is_newline_at_the_end(char *line);
char			*gc_copy(t_db *db, char *s);

/*quoting*/
void			skip_open_spaces(t_quote *quotes, char *line, int *i);
bool			is_quote_oppening(t_quote *head, int i);
void			skip_op(int *i, char *line);

char			**append_word(t_db *db, char **result, char *save);
void			handle_wildcard(t_db *db, char ***result, char *pattern);

t_file_entry	*new_ent_node(t_db *db, struct dirent *ent);
void			add_ent_front(t_file_entry **list, t_file_entry *new);
void			push_ent_back(t_file_entry **list, t_file_entry *new);
t_file_entry	*get_ent_node(t_file_entry *list, char *name,
					unsigned char type);
void			del_ent_node(t_file_entry **list, char *name,
					unsigned char type);

t_str_lst		*new_str_node(t_db *db, char *str);
void			add_str_front(t_str_lst **list, t_str_lst *new);
void			push_str_back(t_str_lst **list, t_str_lst *new);
t_str_lst		*get_str_node(t_str_lst *list, char *str);
void			del_str_node(t_str_lst **list, char *str);

char			*get_environment_var(t_db *db, char *var, char *env[]);
char			*get_env(t_db *db, char *name);
int				valid_char(char c, int index);
int				concat_env_name(char **line, char **env_var_name, int *i,
					t_quote *q);

int				updated_line(t_db *db, char **line, char *variable_name,
					t_str_cut *cut);
// int update_index(t_db *db, char **line, char *value, t_iterators *edges);
int				update_index(t_db *db, char **line, char *value,
					t_str_cut *cut);

bool			split_factor(char *value, char *line, int pos);
bool			is_redir(int op);
int				skip_word(t_db *db, t_quote *quotes, char *line, int *i);
int				skip_white_spaces(t_quote *quotes, char *line, int *i);
int				get_next_tok(t_quote *quotes, char *line, int *i);
char			*get_token_str(t_db *db, int tok);

char			*get_token_error(t_db *db, int tok);
void			init_sx(t_syntax *sx, t_quote *quotes);
int				handle_pipe_syntax(t_syntax *sx, t_quote *quotes, char *line);
int				handle_syntax(t_syntax *sx, t_quote *quotes, char *line);
int				syntax_checker(t_db *db, char *line, t_quote *quotes);
bool			is_redir(int op);
int				skip_word(t_db *db, t_quote *quotes, char *line, int *i);
int				get_next_tok(t_quote *quotes, char *line, int *i);
char			*get_token_str(t_db *db, int tok);
char			*get_token_error(t_db *db, int tok);
int				handle_pipe_sx(int i, bool *found_word, char *line, t_quote *q);
int				h_redir(t_quote *quotes, int tok, int *i, char *line);
int				syntax_checker(t_db *db, char *line, t_quote *quotes);

#endif