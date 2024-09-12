/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils1.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:03 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/12 06:13:22 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

char	*get_environment_var(t_db *db, char *var, char *env[])
{
	int		i;
	char	*temp;

	i = 0;
	temp = ft_strjoin(db, var, "=");
	while (env[i])
	{
		if (ft_strncmp(temp, env[i], ft_strlen(temp)) == 0)
			return (env[i] + ft_strlen(var) + 1);
		i++;
	}
	return (NULL);
}

char	*get_env(t_db *db, char *name)
{
	t_exp_list	*object;

	if (!name)
		return (NULL);
	if (ft_strncmp(name, "?", ft_strlen(name)) == 0)
		return (ft_itoa(db, db->last_status));
	if (ft_strncmp(name, "_", ft_strlen(name)) == 0)
		return (ft_strdup(db, get_env_node(db->env_list, "_")->val));
	if (are_all(name, ft_isdigit))
		return (ft_strdup(db, name + 1));
	object = get_exp_node(db->exp_list, name);
	if (object)
		return (ft_strdup(db, object->val));
	return (ft_strdup(db, ""));
}

bool	valid(char **line, int *i, bool *is_digit, bool *is_special_char)
{
	if (!ft_isalnum((*line)[(*i)]) && (*line)[(*i)] != '?'
		&& (*line)[(*i)] != '_')
		return (false);
	if (*is_special_char)
		return (false);
	if (ft_isdigit((*line)[(*i)]))
		*is_digit = true;
	if ((*line)[(*i)] == '?' || (*line)[(*i)] == '_')
		*is_special_char = true;
	if (ft_isalpha((*line)[(*i)]) && *is_digit)
		return (false);
	return (true);
}

int	concat_env_name(char **line, char **env_var_name, int *i, t_quote *q)
{
	char	*tmp;
	t_db	*db;
	bool	is_digit;
	bool	is_special_char;

	db = this();
	tmp = NULL;
	is_digit = false;
	is_special_char = false;
	while ((*line)[(*i)])
	{
		if (q && is_quote(q, *i))
			return (SUCCESS);
		if (!valid(line, i, &is_digit, &is_special_char))
			break ;
		tmp = concat(db, *env_var_name, (*line)[(*i)]);
		*env_var_name = tmp;
		(*i)++;
	}
	return (SUCCESS);
}
