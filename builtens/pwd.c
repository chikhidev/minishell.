/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:52:15 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 19:55:15 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"

int	pwd_(t_db *db)
{
	char		*pwd;
	t_exp_list	*pwd_exp_node;

	(void)db;
	pwd = getcwd(NULL, 0);
	if (pwd == NULL)
	{
		pwd_exp_node = get_exp_node(db->exp_list, "PWD");
		if (!pwd_exp_node)
			return (1);
		printf("%s\n", pwd_exp_node->val);
	}
	else
		printf("%s\n", pwd);
	free(pwd);
	return (0);
}
