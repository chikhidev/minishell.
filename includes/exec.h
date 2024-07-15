#ifndef EXEC_H
#define EXEC_H

// exec.c
int exec(t_db *db, void **current_node);

// path.c
char    *cmd_path(t_db *db, char *filename);

// path-checker.c
BOOL    is_absolute_path(char *path);
BOOL    is_relative_path(char *path);

#endif