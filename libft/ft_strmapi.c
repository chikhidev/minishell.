/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:20:34 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/15 00:03:36 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*res;
	unsigned int	i;
	unsigned int	len_;

	if (!s || !f)
		return (NULL);
	len_ = (unsigned int)len(s);
	res = malloc(sizeof(char) * (len_ + 1));
	if (!res)
		return (NULL);
	i = 0;
	while (i < len_)
	{
		res[i] = (char)f(i, s[i]);
		i++;
	}
	res[len_] = '\0';
	return (res);
}
