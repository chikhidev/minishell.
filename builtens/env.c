/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:28:14 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:59:10 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

void	write_var(t_db *db, char *key, char *val)
{
	ft_write(db, STDOUT_FILENO, key, (int)len(key));
	ft_write(db, STDOUT_FILENO, "=", 1);
	ft_write(db, STDOUT_FILENO, val, (int)len(val));
	ft_write(db, STDOUT_FILENO, "\n", 1);
}

int	env_(t_db *db, char *av[])
{
	char		*_key;
	char		*_val;
	t_env_list	*curr;

	if (count_array_len(av) > 1)
	{
		put_fd(2, "env: '");
		put_fd(2, av[1]);
		put_fd(2, "': No such file or directory\n");
		return (127);
	}
	curr = db->env_list;
	while (curr)
	{
		if (ft_strcmp(curr->key, "_") == 0)
		{
			_key = curr->key;
			_val = curr->val;
		}
		else
			write_var(db, curr->key, curr->val);
		curr = curr->next;
	}
	write_var(db, _key, _val);
	return (0);
}
