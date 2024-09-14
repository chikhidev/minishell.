/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:48:13 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 09:22:41 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "string.h"

int	paths_getter(t_exp_list *path_node, char ***paths)
{
	char	*path_var;

	if (!path_node)
		path_var = this()->static_path;
	else
		path_var = path_node->val;
	*paths = ft_split(this(), path_var, ":");
	this()->last_status = 127;
	if (!*paths)
		return (FAILURE);
	return (SUCCESS);
}

char	*path_exists(char *filename, int i, t_exp_list *path_node)
{
	char		**paths;
	char		*path;
	char		*tmp;

	if (paths_getter(path_node, &paths) == FAILURE)
		return (NULL);
	while (paths[++i])
	{
		path = ft_strjoin(this(), paths[i], "/");
		tmp = ft_strjoin(this(), path, filename);
		if (access(tmp, F_OK) == 0)
		{
			free_array(this(), paths);
			if (access(tmp, X_OK) != 0)
			{
				this()->last_status = 126;
				error(this(), tmp, "Permission denied");
				return (NULL);
			}
			return (tmp);
		}
	}
	free_array(this(), paths);
	return (NULL);
}

char	*cmd_path(t_db *db, char *filename)
{
	char		*path;
	t_exp_list	*path_node;

	if (!filename)
		return (NULL);
	path_node = get_exp_node(db->exp_list, "PATH");
	if (path_node || db->static_path)
	{
		path = path_exists(filename, -1, path_node);
		if (path)
			return (path);
	}
	else if (access(filename, F_OK) == 0)
	{
		if (access(filename, X_OK) != 0)
		{
			db->last_status = 126;
			error(db, filename, "Permission denied");
			return (NULL);
		}
		db->last_status = 127;
		return (filename);
	}
	return (NULL);
}

bool	is_absolute_path(char *path)
{
	int	i;

	i = 0;
	skip_spaces(path, &i);
	if (path[i] == '/')
		return (true);
	return (false);
}

bool	is_relative_path(char *path)
{
	int	i;

	i = 0;
	skip_spaces(path, &i);
	if (path[i] && i < ((int)ft_strlen(path) - 1) && path[i] == '.'
		&& path[i + 1] == '/')
		return (true);
	return (false);
}
