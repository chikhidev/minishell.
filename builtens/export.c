/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 20:04:20 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:59:10 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"
#include "builtens.h"
#include "string.h"

int	handle_export_args(t_db *db, char *args[], int *status)
{
	int		i;
	char	*key;
	char	*val;
	int		k_len;
	bool	append;

	i = 0;
	while (args[++i])
	{
		k_len = 0;
		append = false;
		val = NULL;
		key = get_key_from_arg(db, args[i], &k_len, &append);
		if ((!good_export_var(key) || k_len < 1) || (append && args[i][k_len
				+ 1] != '='))
			put_exp_err_status(args[i], status);
		else if (args[i][k_len] == '=' || args[i][k_len] == '+')
			assign_append_exp(db, val, args[i], &append);
		else if (args[i][k_len] == '\0' || args[i][k_len] == ' ')
			add_exp(key, val);
		else
			put_exp_err_status(args[i], status);
	}
	return (*status);
}

int	export_(t_db *db, char *args[])
{
	int	n_args;
	int	status;

	status = 0;
	n_args = count_array_len(args);
	if (n_args == 1)
		return (show_export(db));
	return (handle_export_args(db, args, &status));
}
