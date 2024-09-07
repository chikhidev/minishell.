#include "main.h"

#ifndef BUILTENS_H
# define BUILTENS_H

int		echo_(t_db *db, char *args[]);
int		show_export(t_db *db);
int		has_special_char(char *str);
int		good_export_var(char *var);
int		export_(t_db *db, char *args[]);
int		env_(t_db *db, char *args[]);
int		pwd_(t_db *db);
int		exit_(t_db *db, char *args[], int index);
int		cd_(t_db *db, char *args[]);
int		unset_(t_db *db, char *args[]);
bool	is_built_in(void *node);
int		get_key_length(char *arg, bool *append);
int		get_val_length(char *arg, int start_idx);
char	*get_key_from_arg(char *arg, int *k_len, bool *append);
void	fill_key_val(t_db *db, char *arg, char **key, char **val);
char	*get_pwd(t_db *db);
char	*get_val_from_arg(char *arg, int *v_len, int k_len, bool append);
bool	is_built_in(void *node);

#endif