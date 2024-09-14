/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:16:16 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/15 00:01:43 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
#include "libft.h"
#include <stdlib.h>

char	*ft_strdup(t_db *db, const char *s1)
{
	const char	*clone;
	int			len_;

	if (s1 == NULL)
		return (NULL);
	len_ = (int)len(s1);
	clone = gc_malloc(db, sizeof(char) * (len_ + 1));
	if (!clone)
		return (NULL);
	ft_memcpy((void *)clone, s1, len_ + 1);
	return ((char *)clone);
}
