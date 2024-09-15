/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:26:29 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/15 00:02:36 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
#include "libft.h"

char	*ft_substr(t_db *db, char const *s, unsigned int start, size_t len_)
{
	char	*substring;
	size_t	str_len;

	if (!s)
		return (NULL);
	str_len = (int)len(s);
	if (start > str_len || *s == '\0')
		return (ft_strdup(db, ""));
	if (len_ > str_len - start)
		len_ = str_len - start;
	substring = gc_malloc(db, sizeof(char) * (len_ + 1));
	if (!substring)
		return (NULL);
	ft_memcpy(substring, s + start, len_);
	substring[len_] = '\0';
	return (substring);
}

// #include <string.h>
// #include <stdio.h>
// int main()
// {
// 	char *ptr = "hello there!";
// 	printf("%s\n", ft_substr(NULL, 4, 0));
// }