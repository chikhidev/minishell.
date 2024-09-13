/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string7.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:53:34 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/13 17:03:38 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"
#include "string.h"

static int	is_space(char c)
{
	return ((c <= 13 && c >= 9) || c == 32);
}

void	skip_atoll(char *str, int *i, int *sign)
{
	while (is_space(str[(*i)]))
		(*i)++;
	if (str[(*i)] == '-' || str[(*i)] == '+')
	{
		if (str[(*i)] == '-')
			(*sign) = -1;
		(*i)++;
	}
}

long long	ft_atoll(char *str, bool *error)
{
	long long	res;
	int			sign;
	int			i;

	sign = 1;
	i = 0;
	res = 0;
	*error = false;
	skip_atoll(str, &i, &sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res * sign > (LLONG_MAX - (str[i] - '0')) / 10 || res
			* sign < (LLONG_MIN + (str[i] - '0')) / 10)
		{
			*error = true;
			return (0);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	skip_spaces((char *)str, &i);
	if (str[i] != '\0')
		*error = true;
	return (res * sign);
}

void	skip_op(int *i, char *line)
{
	if (line[*i] == '|')
		(*i)++;
	return ;
}
