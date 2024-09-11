/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa_ec.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:30:33 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/11 21:29:14 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "../includes/main.h"
#include "../includes/string.h"

char	*allocate_buffer_ec(t_db *db, long long *number, int *len, int *neg)
{
	char	*res;

	if (*number < 0)
	{
		*neg = 1;
		(*len) += 1;
		*number *= -1;
	}
	res = ec_malloc(db, sizeof(char) * (*len + 1));
	return (res);
}

char	*ft_itoa_ec(t_db *db, int n)
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
	res = allocate_buffer_ec(db, &number, &len, &neg);
	if (!res)
		return (NULL);
	fill_string(number, len, neg, res);
	return (res);
}
