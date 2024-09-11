/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:15:22 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/11 21:46:44 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
#include "libft.h"

static bool	is_separator(char c, char *c2)
{
	int	i;

	i = 0;
	while (c2[i])
	{
		if (c == c2[i])
			return (true);
		i++;
	}
	return (false);
}

static int	count_words(const char *str, char *c)
{
	int				i;
	unsigned int	in_word;

	i = 0;
	in_word = 0;
	while (*str)
	{
		if (!is_separator(*str, c) && in_word == 0)
		{
			in_word = 1;
			i++;
		}
		else if (is_separator(*str, c))
			in_word = 0;
		str++;
	}
	return (i);
}

static char	*extract_word(t_db *db, char const *s, char *c)
{
	int			len;
	char const	*starting;
	char		*word;
	int			i;

	len = 0;
	starting = s;
	i = 0;
	while (*s && !is_separator(*s, c))
	{
		len++;
		s++;
	}
	word = gc_malloc(db, (len + 1) * sizeof(char));
	if (word == NULL)
		return (NULL);
	while (i < len)
	{
		word[i] = starting[i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

static int	fill_and_test(t_db *db, char const *s, char *c, char **result)
{
	int		i;
	char	*word;

	i = 0;
	while (*s)
	{
		if (!is_separator(*s, c))
		{
			word = extract_word(db, s, c);
			result[i++] = word;
			while (*s && !is_separator(*s, c))
				s++;
		}
		else
			s++;
	}
	result[i] = NULL;
	return (1);
}

char	**ft_split(t_db *db, char const *s, char *c)
{
	int		word_count;
	char	**result;

	if (s == NULL)
		return (NULL);
	word_count = count_words(s, c);
	result = gc_malloc(db, (word_count + 1) * sizeof(char *));
	if (result == NULL)
		return (NULL);
	if (fill_and_test(db, s, c, result) == 0)
		return (NULL);
	return (result);
}
