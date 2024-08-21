#include <sys/types.h>
#include <sys/wait.h>
#include "main.h"
#ifndef EXEC_H
#define EXEC_H

#define CHILD 0
// exec.c
// int exec(t_db *db, void **current_node);
int exec(t_db   *db, void *node,  int   **pipes,  int index);
int exec_builtin(t_db   *db,t_cmd_node *node, int   index);
int handle_pipe_op(t_db *db, void *node, int index);
int handle_cmd_node(t_db *db, void *node, int **pipes, int index);
// path.c
char    *cmd_path(t_db *db, char *filename);

// path-checker.c
bool    is_absolute_path(char *path);
bool    is_relative_path(char *path);

void    *ip_add(t_db *db, pid_t  new_ip);
void    ip_free(t_db *db, pid_t  ip_to_free);
void    ip_void(t_db *db);


#endif
