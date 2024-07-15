#ifndef EXEC_H
#define EXEC_H

// path.c
char    *cmd_path(t_db *db, char *filename);

// path-checker.c
BOOL    is_absolute_path(char *path);
BOOL    is_relative_path(char *path);

#endif