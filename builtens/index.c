/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:52:14 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 19:52:15 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "parsing.h"

bool	is_built_in(void *node)
{
	return (ft_strcmp(((t_cmd *)node)->args[0], "echo") == 0
		|| ft_strcmp(((t_cmd *)node)->args[0], "export") == 0
		|| ft_strcmp(((t_cmd *)node)->args[0], "pwd") == 0
		|| ft_strcmp(((t_cmd *)node)->args[0], "env") == 0
		|| ft_strcmp(((t_cmd *)node)->args[0], "cd") == 0
		|| ft_strcmp(((t_cmd *)node)->args[0], "exit") == 0
		|| ft_strcmp(((t_cmd *)node)->args[0], "unset") == 0);
}

int	run_builtin(t_db *db, t_cmd *node, int index)
{
	if (ft_strcmp(node->args[0], "echo") == 0)
		return (echo_(db, node->args));
	else if (ft_strcmp(node->args[0], "export") == 0)
		return (export_(db, node->args));
	else if (ft_strcmp(node->args[0], "pwd") == 0)
		return (pwd_(db));
	else if (ft_strcmp(node->args[0], "env") == 0)
		return (env_(db, node->args));
	else if (ft_strcmp(node->args[0], "cd") == 0)
		return (cd_(db, node->args));
	else if (ft_strcmp(node->args[0], "unset") == 0)
		return (unset_(db, node->args));
	else if (ft_strcmp(node->args[0], "exit") == 0)
		return (exit_(db, node->args, index));
	return (127);
}
