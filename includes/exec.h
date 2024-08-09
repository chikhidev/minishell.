#include <sys/types.h>
#include <sys/wait.h>
#ifndef EXEC_H
#define EXEC_H

#define CHILD 0
// exec.c
// int exec(t_db *db, void **current_node);
int exec(t_db   *db, void *node,    int index);
int exec_builtin(t_db   *db,t_cmd_node *node);
int handle_pipe_op(t_db *db, void *node);
// path.c
int handle_cmd_node(t_db *db, void *node, int index);
char    *cmd_path(t_db *db, char *filename);

// path-checker.c
bool    is_absolute_path(char *path);
bool    is_relative_path(char *path);

#endif