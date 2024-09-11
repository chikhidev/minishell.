/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 18:28:33 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 23:00:25 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "exec.h"
#include "main.h"
#include "string.h"

void	setup_exit(t_db *db, int *args_len, char *av[], int index)
{
	(*args_len) = count_array_len(av);
	if ((*args_len) == 1)
	{
		if (index == -1)
			write(2, "exit\n", 5);
		ft_exit(db, db->last_status, 3, NULL);
	}
}

int	exit_check(char *av[], int args_len, int err, unsigned char *exit_code)
{
	if (err)
	{
		put_fd(2, "exit: ");
		put_fd(2, av[1]);
		put_fd(2, ": numeric argument required\n");
		*exit_code = 2;
	}
	else if (args_len > 2)
	{
		put_fd(2, "exit: too many arguments\n");
		return (1);
	}
	return (0);
}

int	exit_(t_db *db, char *av[], int index)
{
	unsigned char	exit_code;
	int				args_len;
	bool			err;

	err = false;
	exit_code = 0;
	args_len = 1;
	setup_exit(db, &args_len, av, index);
	exit_code = (unsigned char)ft_atoll(av[1], &err);
	if (exit_check(av, args_len, err, &exit_code) == 1)
		return (1);
	if (db->stdin_dup != -1)
		close(db->stdin_dup);
	if (db->stdout_dup != -1)
		close(db->stdout_dup);
	ft_exit(db, exit_code, 3, NULL);
	return (1);
}
