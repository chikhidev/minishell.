/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string5.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:48:14 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:58:35 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

bool	contains_spaces_btwn(char *s)
{
	int	i;

	i = 0;
	skip_spaces(s, &i);
	if (s[i] == '\0')
		return (true);
	return (false);
}

char	*ft_strdup_ec(t_db *db, const char *s1)
{
	const char	*clone;
	int			len_;

	if (s1 == NULL)
		return (NULL);
	len_ = (int)len(s1);
	clone = ec_malloc(db, sizeof(char) * (len_ + 1));
	if (!clone)
		return (NULL);
	ft_memcpy((void *)clone, s1, len_ + 1);
	return ((char *)clone);
}

char	*ft_strjoin_ec(t_db *db, char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		return (ft_strdup_ec(db, s2));
	else if (s1 && !s2)
		return (ft_strdup_ec(db, s1));
	len1 = (int)len(s1);
	len2 = (int)len(s2);
	res = ec_malloc(db, sizeof(char) * (len1 + len2 + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	return (res);
}

int	is_str_empty(t_db *db, char *str)
{
	char	*unquoted;

	unquoted = without_quotes(db, str, NULL);
	if ((int)len(unquoted) == 0)
		return (1);
	return (0);
}

bool	starts_with(char *str, char *sub)
{
	return (ft_strncmp(str, sub, (int)len(sub)) == 0);
}
