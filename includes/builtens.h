#include "main.h"

int echo(t_db *db, char *args[]);
int show_export(t_db *db);
int    has_special_char(char   *str);
int    good_export_var(char    *var);
int export(t_db    *db, char   *args[]);
int    env(t_db *db,   char    *args[]);
int pwd(t_db   *db);
int    cd(t_db *db, char   *args[]);
int unset(t_db *db,    char    *args[]);
BOOL is_built_in(void *node);
int get_key_length(char *arg, bool  *append);
int get_val_length(char *arg,   int start_idx);
char *get_key_from_arg(char *arg,int  *k_len, bool  *append);
BOOL    fill_key_val(t_db   *db,    char  *arg,   char  **key,   char    **val);
char	*get_pwd(t_db	*db);
char    *get_val_from_arg(char  *arg,   int *v_len, int k_len, bool append);
BOOL is_built_in(void *node);