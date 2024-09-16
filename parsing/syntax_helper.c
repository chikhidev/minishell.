/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:35:59 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/16 08:47:56 by sgouzi           ###   ########.fr       */
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
	(void)db;
	(void)quotes;
	while (line[*i] && !is_whitespace(line[*i]) && get_tok(db, line, i,
			quotes) == WORD)
	{
		if (line[*i] == SGL_QUOTE)
		{
			// skip until single quote
			(*i)++;
			while (line[*i] && line[*i] != SGL_QUOTE)
				(*i)++;
			if (line[*i] == SGL_QUOTE)
				(*i)++;
		}
		else if (line[*i] == DBL_QUOTE)
		{
			// skip until double quote
			(*i)++;
			while (line[*i] && line[*i] != DBL_QUOTE)
				(*i)++;
			if (line[*i] == DBL_QUOTE)
				(*i)++;
		}
		else 
			(*i)++;
	}
	if (get_tok(db, line, i, quotes) == WORD)
		return (SUCCESS);
	return (FAILURE);
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
		{
			*i += 1;
			return (APPEND);
		}
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

// get next token with strdup
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

char	*get_token_error(t_db *db, int tok, int next_tok)
{
	if (tok == PIPE)
		return (ft_strdup(db, "|"));
	if (is_redir(tok) && is_redir(next_tok))
		return (get_token_str(db, next_tok));
	else if (is_redir(tok) && next_tok == PIPE)
		return (ft_strdup(db, "|"));
	return (ft_strdup(db, "newline"));
}

int	syntax_checker(t_db *db, char *line, t_quote *quotes)
{
	int		curr;
	int		i;
	int		next_tok;
	char	*msg;
	bool	found_word;

	i = 0;
	curr = INVALID;
	found_word = false;
	skip_open_spaces(quotes, line, &i);
	if (!line[i])
		return (SUCCESS);
	while (line[i])
	{
		curr = get_tok(db, line, &i, quotes);
		if (curr == W_SPACE && line[i] && is_whitespace(line[i]) && ++i)
			continue ;
		if ((curr == WORD || curr == INVALID))
		{
			skip_word(db, quotes, line, &i);
			found_word = true;
			continue ;
		}
		else if (curr != WORD && curr != INVALID) // op
		{
			if (db->heredoc_counter > 16)
				return (error(db, "heredoc",
						"maximum here-document count exceeded"));
			next_tok = get_next_tok(quotes, line, &i);
			if (curr == PIPE)
			{
				msg = ft_strjoin(db, "syntax error near unexpected token `",
						get_token_error(db, curr, next_tok));
				msg = ft_strjoin(db, msg, "'");
				if (!found_word)
					return (error(db, "syntax", msg));
				if (next_tok == INVALID)
					return (error(db, "syntax", msg));
				if (next_tok == PIPE)
					return (error(db, "syntax", msg));
				skip_word(db, quotes, line, &i);
				found_word = true;
				continue ;
			}
			if (is_redir(curr))
			{
				msg = ft_strjoin(db, "syntax error near unexpected token `",
						get_token_error(db, curr, next_tok));
				msg = ft_strjoin(db, msg, "'");
				if (next_tok != WORD)
					return (error(db, "syntax", msg));
				skip_word(db, quotes, line, &i);
				found_word = true;
				continue ;
			}
		}
		i++;
	}
	return (SUCCESS);
}