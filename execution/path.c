/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:48:13 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/12 06:00:51 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "string.h"

void	free_array(t_db *db, char **array)
{
	for (int i = 0; array[i]; i++)
		gc_free(db, array[i]);
	gc_free(db, array);
}

char	*cmd_path(t_db *db, char *filename)
{
	char		**paths;
	char		*path;
	char		*path_var;
	char		*tmp;
	int			i;
	t_exp_list	*path_node;

	if (!filename)
		return (NULL);
	path_node = get_exp_node(db->exp_list, "PATH");
	if (path_node || db->static_path)
	{
		if (!path_node)
			path_var = db->static_path;
		else
			path_var = path_node->val;
		paths = ft_split(db, path_var, ":");
		db->last_status = 127;
		if (!paths)
		{
			return (NULL);
		}
		i = -1;
		while (paths[++i])
		{
			path = ft_strjoin(db, paths[i], "/");
			tmp = ft_strjoin(db, path, filename);
			if (access(tmp, F_OK) == 0)
			{
				free_array(db, paths);
				if (access(tmp, X_OK) != 0)
				{
					db->last_status = 126;
					error(db, tmp, "Permission denied");
					return (NULL);
				}
				return (tmp);
			}
		}
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
	if (path[i] && i < ((int)ft_strlen(path) - 1) && path[i] == '.' && path[i
		+ 1] == '/')
		return (true);
	return (false);
}
