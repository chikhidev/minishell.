#include "main.h"

void echo(char *args[], int n_args);
bool show_export(t_db *db);
bool    has_special_char(char   *str);
bool    good_export_var(char    *var);
bool export(t_db    *db, char   *args[]);
void    env(t_db *db);
void pwd(t_db   *db);
void    cd(t_db *db, char   *args[]);
void unset(t_db *db,    char    *args[]);
BOOL is_built_in(void *node);
int get_key_length(char *arg, bool  *append);
int get_val_length(char *arg,   int start_idx);
char *get_key_from_arg(char *arg,int  *k_len, bool  *append);
BOOL    fill_key_val(t_db   *db,    char  *arg,   char  **key,   char    **val);
char    *get_val_from_arg(char  *arg,   int *v_len, int k_len, bool append);