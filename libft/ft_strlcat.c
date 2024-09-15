/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:19:17 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 23:55:04 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t size)
{
	size_t	dst_len;
	size_t	src_len;
	size_t	src_index;

	dst_len = (int)len(dst);
	src_len = (int)len(src);
	if (size <= dst_len)
		return (size + src_len);
	src_index = 0;
	while (src[src_index] && (dst_len + 1) < size)
	{
		dst[dst_len] = src[src_index];
		src_index++;
		dst_len++;
	}
	dst[dst_len] = '\0';
	return (dst_len + (int)len(&src[src_index]));
}
