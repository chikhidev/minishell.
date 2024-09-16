/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 13:15:22 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/16 20:48:03 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/main.h"
#include "libft.h"
#include "../includes/main.h"
#include "../includes/parsing.h"
#include "../includes/string.h"

static bool	is_separator(char c, char *c2, int pos)
{
	int	i;

	i = 0;
	while (c2[i])
	{
		if (c == c2[i] && !is_inside_quotes_list(this()->q, pos))
			return (true);
		i++;
	}
	return (false);
}

static int	count_words(const char *str, char *c)
{
	int				j;
	int				i;
	unsigned int	in_word;

	i = 0;
	j = 0;
	in_word = 0;
	while (str[j])
	{
		if (!is_separator(str[j], c, j) && !is_quote(this()->q, j))
		{
			if (in_word == 0)
			{
				in_word = 1;
				i++;
			}
		}
		else
			in_word = 0;
		j++;
	}
	return (i);
}

static char	*extract_word(t_db *db, char const *s, char *c, int start)
{
	int			len;
	char		*word;
	int			i;
	int			j;

	len = 0;
	j = start;
	while (s[j] && !is_separator(s[j], c, j))
	{
		if (!is_quote(this()->q, j))
			len++;
		j++;
	}
	word = gc_malloc(db, (len + 1) * sizeof(char));
	i = 0;
	while (i < len)
	{
		if (!is_quote(this()->q, start))
		{
			word[i] = s[start];
			i++;
		}
		start++;
	}
	return (word);
}

static int	fill_and_test(t_db *db, char const *s, char *c, char **result)
{
	int		i;
	int		j;
	char	*word;

	i = 0;
	j = 0;
	while (s[j])
	{
		if (!is_separator(s[j], c, j))
		{
			word = extract_word(db, s, c, j);
			result[i++] = word;
			while (s[j] && !is_separator(s[j], c, j))
				j++;
		}
		else
			j++;
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
