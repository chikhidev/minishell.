#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "../libft/libft.h"

#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE 0
#define NOT_FOUND -1


#define DOBLQUOTE 0
#define SNGLQUOTE 1

/**
 * @details This is the color codes for the shell
*/
#define RED     "\033[0;31m"
#define ORANGE  "\033[0;33m"
#define MAGENTA "\033[0;35m"
#define GREEN   "\033[0;32m"
#define BLUE    "\033[0;34m"
#define CYAN    "\033[0;36m"
#define RESET   "\033[0m"

/**
 * @details This structure is used to store the pointer to free
*/
typedef struct s_gc
{
    void        *ptr;
    struct s_gc *next;
}           t_gc;

/**
 * @details The tree data structure of storing the commands
 * we will have two different types of nodes:
 * 1 - command node => (cmd, args)
 * 2 - operator node => (&&, ||, |, >, <, >>, <<)
*/

#define CMD_NODE 1
#define OP_NODE  2

#define AND_OP   "&&"
#define OR_OP    "||"
#define PIPE_OP  "|"
#define REDIR_OP ">"
#define APPEND_OP ">>"
#define INPUT_OP "<"

/**
 * @details The command node will have the following structure:
 * 1 - type: CMD_NODE or OP_NODE
 * 2 - origin: void* to the origin node
 * 3 - cmd_path: the path of the command
 * 4 - args: the arguments of the command
*/
typedef struct s_cmd_node
{
    int     type; // the common thing between the two nodes
    void    *origin; // the original node
    char    *cmd_path;
    char    **args;
}   t_cmd_node;

/**
 * @details The operator node will have the following structure:
 * 1 - type: CMD_NODE or OP_NODE
 * 2 - origin: void* to the origin node
 * 3 - op: && or || or | or > or < or >> or <<
 * 4 - childs: the children nodes of the operator
 * 5 - n_childs: the number of children to use for etierating
*/
typedef struct s_op_node
{
    int     type; // the common thing between the two nodes
    void    *origin; // the original node
    int     priority;
    int     op_presentation;
    void    **childs;
    int     n_childs;
}   t_op_node;

/**
 * @details The quote structure is used to store the quotes
 * type: DOUBLE => "
 * type: SINLGE => '
*/
typedef struct s_quote
{
    bool    type;
    int     start;
    int     end;
    struct s_quote *next;
}   t_quote;

/**
 * @details The parnth structure is used to store the paranthesis
*/
typedef struct s_parnth
{
    int     open_;
    int     close_;
    struct s_parnth *next;
}   t_parnth;

/**
 * @details The db structure is used to store all data needed
*/
typedef struct s_db
{
    void    *root_node;
    t_gc    *gc;
    char    **env;
    int     op_counter[6];
    t_quote *quotes;
    t_parnth *paranthesis;
}   t_db;

/*prototypes: error.c*/
int error(t_db *db, char *msg);

/*prototypes: memo.c*/
void    *gc_malloc(t_db *db, size_t size);
void    gc_void(t_db *db);
void    gc_free(t_db *db, void *ptr);
void    *gc_realloc(t_db *db, void *ptr, size_t size);

/*prototypes: string.c*/
char    *concat(t_db *db, char *s, char single_char);
#endif