/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:35:07 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/16 20:15:25 by sgouzi           ###   ########.fr       */
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
	int	token;

	token = get_tok(db, line, i, quotes);
	while (line[*i] && !is_whitespace(line[*i]) && token == WORD)
	{
		if (line[*i] == SGL_QUOTE && (*i)++ >= 0)
		{
			while (line[*i] && line[*i] != SGL_QUOTE)
				(*i)++;
			if (line[*i] == SGL_QUOTE)
				(*i)++;
		}
		else if (line[*i] == DBL_QUOTE && (*i)++ >= 0)
		{
			while (line[*i] && line[*i] != DBL_QUOTE)
				(*i)++;
			if (line[*i] == DBL_QUOTE)
				(*i)++;
		}
		else
			(*i)++;
		token = get_tok(db, line, i, quotes);
	}
	return (token);
}

int	skip_white_spaces(t_quote *quotes, char *line, int *i)
{
	int	tok;

	tok = W_SPACE;
	while (line[*i])
	{
		tok = get_tok(this(), line, i, quotes);
		if (tok != W_SPACE)
			break ;
		(*i)++;
	}
	return (tok);
}

int	get_next_tok(t_quote *quotes, char *line, int *i)
{
	(void)quotes;
	if (!line[*i])
		return (INVALID);
	if (line[*i] == '|')
		return (PIPE);
	if (is_whitespace(line[*i]))
		return (W_SPACE);
	if (line[*i] == '>')
	{
		if (line[*i + 1] == '>')
			return (APPEND);
		return (REDIR);
	}
	if (line[*i] == '<')
	{
		if (line[*i + 1] == '<')
			return (HEREDOC);
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
