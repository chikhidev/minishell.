/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.1337.ma>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:18:45 by abchikhi          #+#    #+#             */
/*   Updated: 2024/07/26 13:13:30 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
#include "libft.h"

char	*ft_strjoin(t_db *db, char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		return (ft_strdup(db, s2));
	else if (s1 && !s2)
		return (ft_strdup(db, s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = gc_malloc(db, sizeof(char) * (len1 + len2 + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	return (res);
}

// #include <stdio.h>
// int main(){
// 	printf("Concat: %s",ft_strjoin("hey", NULL));
// }
