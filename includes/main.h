#ifndef MAIN_H
# define MAIN_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/wait.h>
#include <signal.h>



# define SIGNAL unsigned char
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

// triggers
# define CATCH_MALLOC(x) \
	if (!x)             \
		return (error(db, NULL, "Malloc failed7"));
# define CATCH(x, message) \
	if (x == FAILURE)     \
		return (error(db, NULL, message));
# define CATCH_ONFAILURE(x, return_) \
	if (x == FAILURE)               \
		return (return_);
# define CATCH_ONfalse(x, return_) \
	if (x == false)               \
		return (return_);
# define CATCH_ONINVALID(x, return_) \
	if (x == INVALID)               \
		return (return_);
# define CATCH_ONNOTFOUND(x, return_) \
	if (x == NOT_FOUND)              \
		return (return_);

# define CATCH_ERROR \
	if (db->error)  \
		return (FAILURE);

# define DOBLQUOTE 34
# define SNGLQUOTE 39

# define CURR_OP ((t_op_node *)*current_node)
# define CURR_CMD ((t_cmd_node *)*current_node)
# define OP ((t_op_node *)node)
# define CMD ((t_cmd_node *)node)

# define IS_CHILD (pid == 0)
# define IS_PARENT (pid > 0)
# define IS_ERROR (pid < 0)

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
# define BOLD      "\033[1m"
# define UNDERLINE "\033[4m"


typedef struct s_iterators
{
	int					i;
	int					j;
}						t_iterators;

/*file types*/
# define INPUTFILE 0
# define OUTPUTFILE 1
# define APPENDFILE 2

typedef struct s_op_node
{
	int type;     // the common thing between the two nodes
	int					op_presentation;
	void				**childs;
	int					n_childs;

	int input_fd;
	int output_fd;

	// execution part ------ <<<<<<
}						t_op_node;

typedef struct s_int
{
	int					n;
	struct s_int	*next;
}						t_int;



typedef struct s_here_doc /*here doc saver*/
{
	t_op_node *ptr;
	struct s_here_doc *next;
}						t_here_doc;

typedef struct s_env_list
{
	char					*key;
	char					*val;
	bool					has_val;
	struct s_env_list		*next;
}   t_env_list;

typedef struct s_exp_list
{
	char					*key;
	char					*val;
    bool                    visible;
	struct s_exp_list		*next;
}   t_exp_list;

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
	struct dirent				*entry;
	struct s_file_entry			*next;
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
	int 				type;     // the common thing between the two nodes

	// this is gonna be only in the child proccess just when it gonna e executed!!!
	char				**args;
	int					input_fd;
	int					output_fd;

}						t_cmd_node;

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
	char	*str;
	struct s_str_lst	*next;
}	t_str_lst;

/**
 * @details The db structure is used to store all data needed
 */
typedef struct s_db
{
	int					debug;
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

	/*io*/
	int					heredoc_counter;
	int					curr_type;
	int					input_fd;
	int					output_fd;
	int					stdin_dup;
	int					stdout_dup;
	int					pipe[2];
	int					read_fd;

	// has the permission to run the line? by default yes.
	bool				exec_line;

    /*local envirement variables*/
	t_env_list			*env_list;
	t_int				*pid;
	t_int				*fd;
	t_exp_list			*exp_list;
	char				*static_path;
}						t_db;

/*prototypes: error.c*/
int						error(t_db *db, char *specifier, char *message);

/*prototypes: memo.c*/

void    *gc_malloc(t_db *db, size_t size);
void    *ec_malloc(t_db *db, size_t size);
void    gc_free(t_db *db, void *ptr);
void    ec_free(t_db *db, void *ptr);
void    gc_void(t_db *db);
void    ec_void(t_db *db);
void    *gc_realloc(t_db *db, void *ptr, size_t size);
void    *fd_add(t_db *db, pid_t  new_fd);
void    fd_free(t_db *db, pid_t  fd_to_free);
void    fd_void(t_db *db);
int		ft_open(t_db *db, char *file, int flags, int type);
void	ft_exit(t_db *db, int status, short free_flag, char *msg);
void	ft_close(t_db *db, int *fd);
void	ft_pipe(t_db *db, int *pipe_fd);
void	ft_dup2(t_db *db, int old_fd, int new_fd);
void	ft_write(t_db *db, int fd, char *msg, int len);
/*prototypes: string.c*/
int						count(char *line, char c);
char					*concat(t_db *db, char *s, char single_char);

int						is_op3(char *line, int *i);

bool					contains_spaces_btwn(char *s);

t_env_list				*new_env_node(t_db *db, char   *key, char	*val);
void					add_env_front(t_env_list  **list,   t_env_list	*new);
void					push_env_back(t_env_list  **list, t_env_list	*new);
void					*push_sort(t_db *db, t_env_list  **list, char    *data);

t_exp_list				*new_exp_node(t_db *db, char   *key, char    *val);
void					add_exp_front(t_exp_list  **list,   t_exp_list	*new);
void					push_exp_sort(t_exp_list  **list,  t_exp_list	*new);
void                    push_exp_back(t_exp_list  **list,  t_exp_list	*new);
t_exp_list              *get_exp_node(t_exp_list    *list,  char    *key);
// void					free_environment(t_db  *db);
t_env_list              *get_env_node(t_env_list    *list,  char    *key);
void					del_env_node(t_env_list    **list,  char    *key);
void					del_exp_node(t_exp_list    **list,  char    *key);
char					**env_list_to_env_arr(t_db	*db);

void					catch_feedback(t_db *db, int process_res);

/*signals*/
void default_signals_behav(bool ignore_quit);
void handle_parent_signals(void);
void heredoc_signals_handling(void);


/* FUNCTIONS */

#endif
