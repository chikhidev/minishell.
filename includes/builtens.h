/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtens.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:11:27 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 21:12:52 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTENS_H
# define BUILTENS_H

# include "main.h"

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
void	fill_key_val(t_db *db, char *arg, char **key, char **val);
char	*get_pwd(t_db *db);
char	*get_val_from_arg(char *arg, int *v_len, int k_len, bool append);
bool	is_built_in(void *node);
int		handle_export_args(t_db *db, char *args[], int *status);
int		show_export(t_db *db);
int		has_special_char(char *str);
int		good_export_var(char *var);
int		get_key_length(char *arg, bool *append);
int		get_val_length(char *arg, int start_idx);
char	*get_key_from_arg(t_db *db, char *arg, int *k_len, bool *append);
int		affect_exp_node_val(t_db *db, t_exp_list *node, bool append, char *val);
int		affect_env_node_val(t_db *db, t_env_list *node, bool append, char *val);
void	fill_key_val(t_db *db, char *arg, char **key, char **val);
void	handle_add_env_export(t_db *db, char *key, char *val);
void	handle_add_exp_export(t_db *db, char *key, char *val);
void	put_exp_err_status(char *arg, int *status);
void	assign_append_exp(t_db *db, char *val, char *token, bool *append);
void	add_exp(char *key, char *val);

#endif