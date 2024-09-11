/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 19:08:34 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/11 19:55:37 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtens.h"
#include "main.h"
#include "parsing.h"
#include "string.h"

char	*get_key_from_arg(t_db *db, char *arg, int *k_len, bool *append)
{
	char	*key;

	*k_len = get_key_length(arg, append);
	key = ec_malloc(db, (*k_len + 1) * sizeof(char));
	ft_strlcpy(key, arg, *k_len + 1);
	return (key);
}

char	*get_val_from_arg(char *arg, int *v_len, int k_len, bool append)
{
	char	*val;
	t_db	*db;
	int		offset;

	db = this();
	offset = 0;
	if (append)
		offset = 1;
	*v_len = get_val_length(arg, k_len + 1 + offset);
	val = ec_malloc(db, (*v_len + 1) * sizeof(char));
	ft_strlcpy(val, &arg[k_len + offset + 1], *v_len + 1);
	return (val);
}

int	affect_exp_node_val(t_db *db, t_exp_list *node, bool append, char *val)
{
	char	*joined;

	if (node)
	{
		if (append)
		{
			joined = ft_strjoin_ec(db, node->val, val);
			if (!joined)
				return (false);
			node->val = joined;
		}
		else
		{
			node->val = val;
		}
	}
	return (SUCCESS);
}

int	affect_env_node_val(t_db *db, t_env_list *node, bool append, char *val)
{
	char	*joined;

	if (node)
	{
		if (append)
		{
			joined = ft_strjoin_ec(db, node->val, val);
			if (!joined)
				return (false);
			node->val = joined;
		}
		else
			node->val = val;
	}
	return (SUCCESS);
}

void	fill_key_val(t_db *db, char *arg, char **key, char **val)
{
	bool	append;
	int		k_len;
	int		v_len;

	v_len = 0;
	k_len = 0;
	append = false;
	*key = get_key_from_arg(db, arg, &k_len, &append);
	*val = get_val_from_arg(arg, &v_len, k_len, append);
}
