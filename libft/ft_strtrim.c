/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:21:57 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/15 00:02:09 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
#include "libft.h"

static int	is_set(char c, const char *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

char	*ft_strtrim(t_db *db, const char *s1, const char *set)
{
	int		front;
	int		rear;
	int		len_;
	char	*res;

	if (!s1)
		return (NULL);
	if (!set)
		return (ft_strdup(db, s1));
	front = 0;
	while (s1[front] && is_set(s1[front], set))
		front++;
	rear = (int)len(s1) - 1;
	while (rear > front && is_set(s1[rear], set))
		rear--;
	len_ = rear - front + 1;
	if (len_ <= 0)
		return (ft_strdup(db, ""));
	res = malloc(sizeof(char) * (len_ + 1));
	if (res == NULL)
		return (NULL);
	ft_strlcpy(res, s1 + front, len_ + 1);
	return (res);
}
