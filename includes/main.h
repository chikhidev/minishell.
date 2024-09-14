/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:01:39 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 17:40:00 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MAIN_H
# define MAIN_H

# include "../libft/libft.h"
# include <ctype.h>
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SUCCESS 1
# define FAILURE 0
# define NOT_FOUND -1
# define INVALID -1
# define MISSING_FILE -2
# define PERMISSION_DENIED -3
# define VALIDE 1
# define PASS
# define GOOD 1
# define BAD 0
# define CLOSED -2
# define FAIL 150

# define CHILD 0

// triggers
# define DOBLQUOTE 34
# define SNGLQUOTE 39

/**
 * @details This is the color codes for the shell
 */
# define ORANGE "\033[0;33m"
# define MAGENTA "\033[0;35m"
# define GREEN "\033[1;32m"
# define RED "\033[1;31m"
# define BLUE "\033[0;34m"
# define CYAN "\033[0;36m"
# define RESET "\033[0m"
# define BOLD "\033[1m"
# define UNDERLINE "\033[4m"

typedef struct s_iterators
{
	int					i;
	int					j;
}						t_iterators;

typedef struct str_cut
{
	int					start_ignore;
	int					end_ignore;

	int					start_include;
	int					end_include;

}						t_str_cut;

/*file types*/
# define INPUTFILE 0
# define OUTPUTFILE 1
# define APPENDFILE 2

typedef struct s_op_node
{
	int					type;
	int					op_presentation;
	void				**childs;
	int					n_childs;
	int					input_fd;
	int					output_fd;
}						t_op;

typedef struct s_int
{
	int					n;
	struct s_int		*next;
}						t_int;

typedef struct s_here_doc /*here doc saver*/
{
	t_op				*ptr;
	struct s_here_doc	*next;
}						t_here_doc;

typedef struct s_env_list
{
	char				*key;
	char				*val;
	bool				has_val;
	struct s_env_list	*next;
}						t_env_list;

typedef struct s_exp_list
{
	char				*key;
	char				*val;
	struct s_exp_list	*next;
}						t_exp_list;

/**
 * @details This structure is used to store the pointer to free
 */
typedef struct s_gc
{
	void				*ptr;
	struct s_gc			*next;
}						t_gc;

typedef struct s_file_entry
{
	struct dirent		*entry;
	struct s_file_entry	*next;
}						t_file_entry;

/**
 * @details The tree data structure of storing the commands
 * we will have two different types of nodes:
 * 1 - command node => (cmd, args)
 * 2 - operator node => (&&, ||, |)
 */

# define CMD_NODE 1
# define OP_NODE 2

/**
 * @details The command node will have the following structure:
 * 1 - type: CMD_NODE or OP_NODE
 * 2 - origin: void* to the origin node
 * 3 - cmd_path: the path of the command
 * 4 - args: the arguments of the command
 */
typedef struct s_cmd_node
{
	int					type;
	char				**args;
	int					input_fd;
	int					output_fd;

}						t_cmd;

/**
 * @details The operator node will have the following structure:
 * 1 - type: CMD_NODE or OP_NODE
 * 2 - origin: void* to the origin node
 * 3 - op: && or || or | or > or < or >> or <<
 * 4 - childs: the children nodes of the operator
 * 5 - n_childs: the number of children to use for etierating
 */

typedef struct s_operators
{
	int					i;
	char				*name;
	int					size;
	struct s_operators	*next;
}						t_operators;

/**
 * @details The quote structure is used to store the quotes

 * type: DOUBLE => "
 * type: SINLGE => '
*/

typedef struct s_quote
{
	int					ascii;
	int					start;
	int					end;
	struct s_quote		*next;
}						t_quote;

typedef struct s_tracker
{
	t_quote				*quotes;
}						t_tracker;

typedef struct s_str_lst
{
	char				*str;
	struct s_str_lst	*next;
}						t_str_lst;

typedef struct s_tokenizer
{
	t_quote				**quotes;
	char				*line;
	char				*save;
	char				**result;
	bool				read_write_perm;
	t_iterators			it;
}						t_tokenizer;

/**
 * @details The db structure is used to store all data needed
 */
typedef struct s_db
{
	int					debug;
	bool				ctrl_c;
	/*tree head*/
	/*tree head*/
	void				*root_node;
	/*momory management*/
	t_gc				*gc;
	t_gc				*ec;
	/*storing init data*/
	char				**env;
	/*storing in tracked signals*/
	int					last_status;

	/*error flag*/
	bool				error;
	/**process id(s) for childs*/
	int					*pids;

	/*expand tells wither to expand or not*/
	bool				split;

	/*io*/
	int					heredoc_counter;
	int					curr_type;
	int					input_fd;
	int					output_fd;
	int					stdin_dup;
	int					stdout_dup;
	int					pipe[2];
	int					read_fd;

	bool				is_path_unset;

	// has the permission to run the line? by default yes.
	bool				exec_line;

	int					expand_hrdc;

	/*local envirement variables*/
	t_env_list			*env_list;
	t_int				*pid;
	t_int				*fd;
	t_exp_list			*exp_list;
	char				*static_path;
}						t_db;

/*prototypes*/
/**

	* @details This function is used to initialize the db structure
			and call be called whenever needed to set or read the data
 */
t_db					*this(void);

/*prototypes: error.c*/
int						error(t_db *db, char *specifier, char *message);

/*prototypes: memo.c*/

void					*gc_malloc(t_db *db, size_t size);
void					*ec_malloc(t_db *db, size_t size);
void					gc_free(t_db *db, void *ptr);
void					ec_free(t_db *db, void *ptr);
void					gc_void(t_db *db);
void					ec_void(t_db *db);
void					*gc_realloc(t_db *db, void *ptr, size_t old_size,
							size_t size);
void					*fd_add(t_db *db, pid_t new_fd);
void					fd_free(t_db *db, pid_t fd_to_free);
void					fd_void(t_db *db);
int						ft_open(t_db *db, char *file, int flags, int type);
void					ft_exit(t_db *db, int status, short free_flag,
							char *msg);
void					ft_close(t_db *db, int *fd);
void					ft_pipe(t_db *db, int *pipe_fd);
void					ft_dup2(t_db *db, int old_fd, int new_fd);
int						ft_write(t_db *db, int fd, char *msg, int len);
/*prototypes: string.c*/
void					ft_dup2(t_db *db, int old_fd, int new_fd);
int						ft_dup(t_db *db, int fd);
int						count(char *line, char c);
char					*concat(t_db *db, char *s, char single_char);

bool					contains_spaces_btwn(char *s);

t_env_list				*set_env_lst(t_db *db, char *env[]);
t_env_list				*new_env_node(t_db *db, char *key, char *val);
void					add_env_front(t_env_list **list, t_env_list *new);
void					push_env_back(t_env_list **list, t_env_list *new);

t_exp_list				*new_exp_node(t_db *db, char *key, char *val);
void					add_exp_front(t_exp_list **list, t_exp_list *new);
void					push_exp_sort(t_exp_list **list, t_exp_list *new);
void					push_exp_back(t_exp_list **list, t_exp_list *new);
t_exp_list				*get_exp_node(t_exp_list *list, char *key);
t_env_list				*get_env_node(t_env_list *list, char *key);
void					del_env_node(t_env_list **list, char *key);
void					del_exp_node(t_exp_list **list, char *key);
char					**env_list_to_env_arr(t_db *db);

void					catch_feedback(t_db *db, int process_res);

/*signals*/
void					default_signals_behav(void);
void					handle_parent_signals(void);
void					handle_here_doc_signals(void);
void					handle_sigint(int signum);
void					handle_here_doc_signals(void);
t_env_list				*set_default_env(t_db *db);
t_exp_list				*set_default_exp(t_db *db);
t_exp_list				*set_exp_lst(t_db *db, char *env[]);
void					handle_pwd_oldpwd_no_exist(t_db *db, bool pwd_exist,
							bool old_pwd_exist);
void					handle_underscore_no_exist(t_db *db);
void					handle_pwd(t_db *db);
void					handle_shell_level(t_db *db, char *key, char **val);
void					default_signals_behav(void);
void					parent_behav(int signal);
void					handle_parent_signals(void);
void					heredoc_behave(int signal);
void					handle_here_doc_signals(void);

void					skip_open_spaces(t_quote *quotes, char *line, int *i);
void					add(t_db *db, char ***result, char *save);

int						handle_prompt(t_db *db, char **line);

void					free_array(t_db *db, char **array);
/* FUNCTIONS */

#endif
