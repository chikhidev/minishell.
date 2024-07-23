#include "main.h"

void echo(char *args[], int n_args);
bool only_new_line(char   *arg);
bool show_export(t_db *db);
bool    has_special_char(char   *str);
bool    good_export_var(char    *var);
bool export(t_db    *db, char   *args[]);
void    env(t_db *db);
void pwd(t_db   *db);
void    cd(t_db *db, char   *args[]);