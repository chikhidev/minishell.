/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_striteri.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:16:35 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/15 00:03:57 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_striteri(char *s, void (*f)(unsigned int, char *))
{
	unsigned int	i;
	unsigned int	len_;

	if (!s || !f)
		return ;
	len_ = (unsigned int)len(s);
	i = 0;
	while (i < len_)
	{
		f(i, &s[i]);
		i++;
	}
}
