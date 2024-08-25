/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:16:16 by abchikhi          #+#    #+#             */
/*   Updated: 2023/11/02 13:16:18 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdlib.h>
#include "../includes/main.h"

char	*ft_strdup(t_db	*db,	const char *s1)
{
	const char	*clone;
	int			len;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	clone = gc_malloc(db,	sizeof(char) * (len + 1));
	if (!clone)
		return (NULL);
	ft_memcpy((void *)clone, s1, len + 1);
	return ((char *)clone);
}
