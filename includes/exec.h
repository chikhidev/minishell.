/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:13:27 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/15 00:13:17 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H
# define CHILD 0

# include "main.h"
# include <sys/types.h>
# include <sys/wait.h>
// exec.c
void	exec(t_db *db, void *node);
void	handle_pipe_op(t_db *db, void *node);
void	handle_cmd_node(t_db *db, void *node, int **pipes, int index);
// path.c

int		run_builtin(t_db *db, t_cmd *node, int index);
char	*cmd_path(t_db *db, char *filename);

// path-checker.c
bool    is_path(char *path);

void	*pid_add(t_db *db, pid_t new_ip);
void	pid_free(t_db *db, pid_t ip_to_free);
void	pid_void(t_db *db);
void	handle_underscore(t_db *db, void *node);

int		**prepare_pipes(t_db *db, int n_pipes);
int		close_all_pipes(t_db *db, int **pipes);
int		dup_cmd_io(t_db *db, t_cmd *command);
int		get_pipes_count(int **pipes);
int		dup_pipes(t_db *db, int **pipes, int index);
void	exec_cmd(t_db *db, void *node, int **pipes, int index);
int		handle_single_builtin(t_db *db, void *node, int index);
int		handle_builtin(t_db *db, void *node, int **pipes, int index);
void	handle_cmd_node(t_db *db, void *node, int **pipes, int index);

void	handle_is_dir(t_db *db, char *arg);
void	handle_underscore(t_db *db, void *node);
void	waiter(t_db *db);

char	*get_path(t_db *db, char **args);
void	handle_pipe_op(t_db *db, void *node);
void	exec(t_db *db, void *node);

#endif
