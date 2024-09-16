/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   string6.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:48:14 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/16 15:26:58 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"
#include "string.h"

int	get_tok(t_db *db, char *s, int *i, t_quote *quotes)
{
	(void)quotes;
	if (s[*i] == '\0')
		return (INVALID);
	if (is_whitespace(s[*i]))
		return (W_SPACE);
	if (s[*i] == '|')
		return ((*i)++, PIPE);
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
			return (((*i) += 2), db->heredoc_counter++, HEREDOC);
		return ((*i)++, INPUT);
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
			return (((*i) += 2), APPEND);
		return ((*i)++, REDIR);
		return (REDIR);
	}
	return (WORD);
}

bool	is_open_whitespace(char *line, int i, t_quote *quotes)
{
	return (is_whitespace(line[i]) && !is_inside_quotes_list(quotes, i));
}

bool	is_open_io(char *line, int i, t_quote *quotes)
{
	return ((validate_io(&line[i], 1) != INVALID || validate_io(&line[i],
				2) != INVALID) && !is_inside_quotes_list(quotes, i));
}

void	put_fd(int fd, char *s)
{
	t_db	*db;

	db = this();
	while (*s)
	{
		ft_write(db, fd, s, 1);
		s++;
	}
}
