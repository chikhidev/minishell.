/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_1.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:08:35 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 19:08:44 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

int	show_export(t_db *db)
{
	int			status;
	t_exp_list	*vars;
	t_exp_list	*curr;

	status = 0;
	vars = db->exp_list;
	curr = vars;
	while (curr)
	{
		if (ft_strcmp(curr->key, "_") != 0)
		{
			printf("declare -x %s", curr->key);
			if (curr->val)
				printf("=\"%s\"", curr->val);
			printf("\n");
		}
		curr = curr->next;
	}
	return (status);
}

int	has_special_char(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_' && str[i] != '+'
			&& str[i] != '=')
			return (true);
		i++;
	}
	return (false);
}

int	good_export_var(char *var)
{
	if (var && (ft_isdigit(var[0])))
		return (false);
	else if (!ft_isalpha(var[0]) && var[0] != '_')
		return (false);
	else if (has_special_char(var))
		return (false);
	else
		return (true);
	return (true);
}

int	get_key_length(char *arg, bool *append)
{
	int	i;

	i = 0;
	while (arg[i])
	{
		if (arg[i] == '+')
		{
			*append = true;
			return (i);
		}
		if (arg[i] == '=' || arg[i] == '\0')
			return (i);
		i++;
	}
	return (i);
}

int	get_val_length(char *arg, int start_idx)
{
	int	i;
	int	len;

	len = 0;
	i = start_idx;
	while (arg[i])
	{
		if (arg[i] == '\0')
			return (len);
		i++;
		len++;
	}
	return (len);
}
