/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:30:33 by abchikhi          #+#    #+#             */
/*   Updated: 2024/08/18 15:56:42 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "../includes/main.h"

static int	ft_intlen(int n)
{
	int	len;

	len = 0;
	while (n)
	{
		n /= 10;
		len++;
	}
	return (len);
}

static char	*allocate_buffer(t_db *db, long long *number, int *len, int *neg)
{
	char	*res;

	if (*number < 0)
	{
		*neg = 1;
		(*len) += 1;
		*number *= -1;
	}
	res = gc_malloc(db, sizeof(char) * (*len + 1));
	return (res);
}

static void	fill_string(long long number, int len, int neg, char *string)
{
	string[len--] = '\0';
	if (number <= 9)
		string[len--] = number + 48;
	else
	{
		while (len >= 0)
		{
			string[len--] = (number % 10) + 48;
			number /= 10;
		}
	}
	if (neg)
		string[0] = '-';
}

char	*ft_itoa(t_db *db, int n)
{
	int			len;
	char		*res;
	int			neg;
	long long	number;

	len = ft_intlen(n);
	neg = 0;
	number = (long long)n;
	if (number == 0)
		return ((char *)ft_strdup(db, "0"));
	res = allocate_buffer(db, &number, &len, &neg);
	if (!res)
		return (NULL);
	fill_string(number, len, neg, res);
	return (res);
}
