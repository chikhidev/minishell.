#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>

#include "../libft/libft.h"

#define BOOL unsigned char
#define TRUE 1
#define FALSE 0
#define SUCCESS 1
#define FAILURE 0
#define NOT_FOUND -1
#define INVALID -1
#define VALIDE 1
#define PASS

#define DOBLQUOTE 34
#define SNGLQUOTE 39

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

typedef struct s_iterators
{
    int i;
    int j;
} t_iterators;

typedef struct s_op_node
{
    int     type; // the common thing between the two nodes
    void    *origin; // the original node
    int     priority;
    int     op_presentation;
    void    **childs;
    int     n_childs;
}   t_op_node;

typedef struct s_here_doc /*here doc saver*/
{
    t_op_node    *ptr;
    struct s_here_doc *next;
}   t_here_doc;

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

typedef struct  s_operators
{
    int i;
    char    *name;
    int     size;
    struct s_operators  *next; 
}   t_operators;

/**
 * @details The quote structure is used to store the quotes
 
 * type: DOUBLE => "
 * type: SINLGE => '
*/

typedef struct s_quote
{
    int     ascii;
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

typedef struct s_tracker
{
    t_parnth    *paranthesis;
    t_quote     *quotes;
}   t_tracker;

/**
 * @details The db structure is used to store all data needed
*/
typedef struct s_db
{
    int     debug;
    void    *root_node;
    t_gc    *gc;
    char    **env;
    int     op_counter[6];
    int     last_signal;
    t_operators *ops;
    t_here_doc  *here_docs;
}   t_db;

/*prototypes: error.c*/
int error(t_db *db, char *msg);

/*prototypes: memo.c*/
void    *gc_malloc(t_db *db, size_t size);
void    gc_void(t_db *db);
void    gc_free(t_db *db, void *ptr);
void    *gc_realloc(t_db *db, void *ptr, size_t size);

/*prototypes: string.c*/
int     count(char *line, char c);
char    *concat(t_db *db, char *s, char single_char);
char    *remove_paranthesis(t_db *db, char *line, t_parnth *local_paranths);

#endif