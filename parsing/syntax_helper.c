/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:35:59 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/15 01:37:09 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

bool	is_redir(int op)
{
	return (op == APPEND || op == INPUT || op == HEREDOC || op == REDIR);
}

int	skip_word(t_db *db, t_quote *quotes, char *line, int *i)
{
	while (line[*i] && !is_whitespace(line[*i]) && get_tok(db, line, i,
			quotes) == WORD)
	{
		if (line[*i] == SGL_QUOTE)
		{
			(*i)++;
			while (line[*i] && line[*i] != SGL_QUOTE)
				(*i)++;
			if (line[*i] == SGL_QUOTE)
				(*i)++;
		}
		else if (line[*i] == DBL_QUOTE)
		{
			(*i)++;
			while (line[*i] && line[*i] != DBL_QUOTE)
				(*i)++;
			if (line[*i] == DBL_QUOTE)
				(*i)++;
		}
		else 
			(*i)++;
	}
	return	(get_tok(db, line, i, quotes));
}

int	get_next_tok(t_quote *quotes, char *line, int *i)
{
	if (line[*i])
		++(*i);
	skip_open_spaces(quotes, line, i);
	if (!line[*i])
		return (INVALID);
	if (line[*i] == '|')
		return (PIPE);
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
			return ((*i += 1), APPEND);
		return (REDIR);
	}
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
		{
			*i += 1;
			return (HEREDOC);
		}
		return (INPUT);
	}
	return (WORD);
}

char	*get_token_str(t_db *db, int tok)
{
	if (tok == PIPE)
		return (ft_strdup(db, "|"));
	if (tok == APPEND)
		return (ft_strdup(db, ">>"));
	if (tok == REDIR)
		return (ft_strdup(db, ">"));
	if (tok == INPUT)
		return (ft_strdup(db, "<"));
	if (tok == HEREDOC)
		return (ft_strdup(db, "<<"));
	return (NULL);
}
