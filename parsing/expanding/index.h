#ifndef INDEX_H
# define INDEX_H

char    *get_environment_var(t_db *db, char   *var, char *env[]);
char *get_env(t_db *db, char *name);
int valid_char(char c, int  index);
int concat_env_name(t_db *db, char **line, char **env_var_name, int *i);
int updated_line(t_db *db, char **line, char *variable_name, t_iterators *reminder);
int update_index(t_db *db, char **line, char *value, t_iterators *edges);

#endif