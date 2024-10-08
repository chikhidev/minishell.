/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 17:29:55 by abchikhi          #+#    #+#             */
/*   Updated: 2023/11/15 01:48:55 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

void	*ft_calloc(size_t count, size_t size)
{
	void	*ptr;
	size_t	nbytes;

	nbytes = count * size;
	ptr = (void *)malloc(nbytes);
	if (!ptr)
		return (NULL);
	ft_bzero(ptr, nbytes);
	return (ptr);
}
