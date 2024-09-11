/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:15:58 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/11 21:47:34 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c, int *i)
{
	int	j;

	j = *i;
	while (s[j])
	{
		if (s[j] == (char)c)
		{
			*i = j;
			return ((char *)s + j);
		}
		j++;
	}
	if (s[j] == (char)c)
	{
		*i = j;
		return ((char *)s + j);
	}
	return (NULL);
}

char	*ft_strsearch(const char *s, int c)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			return ((char *)s + i);
		i++;
	}
	if (s[i] == (char)c)
		return ((char *)s + i);
	return (NULL);
}
