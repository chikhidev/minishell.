/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:52:17 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 19:52:58 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include <string.h>

int	unset_(t_db *db, char *args[])
{
	int	i;

	i = 1;
	while (args[i])
	{
		if (ft_strcmp(args[i], "_") != 0)
		{
			del_env_node(&db->env_list, args[i]);
			del_exp_node(&db->exp_list, args[i]);
		}
		if (ft_strcmp(args[i], "PATH") == 0)
			db->static_path = NULL;
		i++;
	}
	return (0);
}
