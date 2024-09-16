/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax_helper.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgouzi <sgouzi@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:35:59 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/16 20:43:40 by sgouzi           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

char	*get_token_error(t_db *db, int tok)
{
	if (tok == PIPE)
		return (ft_strdup(db, "syntax error near unexpected token `|'"));
	if (tok == APPEND)
		return (ft_strdup(db, "syntax error near unexpected token `>>'"));
	if (tok == REDIR)
		return (ft_strdup(db, "syntax error near unexpected token `>'"));
	if (tok == INPUT)
		return (ft_strdup(db, "syntax error near unexpected token `<'"));
	if (tok == HEREDOC)
		return (ft_strdup(db, "syntax error near unexpected token `<<'"));
	return (ft_strdup(this(), "syntax error near unexpected token `newline'"));
}

int	handle_pipe_sx(int i, bool *found_word, char *line, t_quote *quotes)
{
	int	next;

	if (!*found_word)
		return (FAILURE);
	next = get_next_tok(quotes, line, &i);
	if (next == W_SPACE)
	{
		next = skip_white_spaces(quotes, line, &i);
		if (next != WORD && next != REDIR && next != APPEND && next != HEREDOC
			&& next != INPUT)
			return (FAILURE);
	}
	if (next != WORD && next != REDIR && next != APPEND && next != HEREDOC
		&& next != INPUT)
		return (FAILURE);
	*found_word = false;
	return (SUCCESS);
}

int	h_redir(t_quote *quotes, int tok, int *i, char *line)
{
	char	*msg;
	int		next;

	if (this()->heredoc_counter > 16)
		return (this()->last_status = 2, error(this(), "syntax",
				"maximum here-document count exceeded"));
	msg = get_token_error(this(), tok);
	if (msg == NULL)
		return (FAILURE);
	next = get_next_tok(quotes, line, i);
	if (next == W_SPACE)
	{
		next = skip_white_spaces(quotes, line, i);
		if (next != WORD)
			return (error(this(), "syntax", msg));
	}
	if (next != WORD)
		return (error(this(), "syntax", msg));
	return (SUCCESS);
}

/*
	4 -> REDIR
	5 -> APPEND
	6 -> INPUT
	7 -> HEREDOC
	8 -> W_SPACE
	9 -> WORD
*/

void	init_syntax(int *i, bool *found_word)
{
	*i = 0;
	*found_word = false;
}

int	syntax_checker(t_db *db, char *line, t_quote *q)
{
	t_syntax	sx;

	init_syntax(&sx.i, &sx.found_word);
	sx.curr = get_tok(db, line, &sx.i, q);
	while (sx.curr != INVALID)
	{
		if (sx.curr == W_SPACE)
			sx.curr = skip_white_spaces(q, line, &sx.i);
		if (sx.curr == WORD)
		{
			sx.curr = skip_word(db, q, line, &sx.i);
			sx.found_word = true;
		}
		if (is_redir(sx.curr) && h_redir(q, sx.curr, &sx.i, line) == FAILURE)
			return (FAILURE);
		else if (sx.curr == PIPE)
		{
			if (handle_pipe_sx(sx.i, &sx.found_word, line, q) == FAILURE)
				return (error(db, "syntax",
						"syntax error near unexpected token `|'"));
		}
		sx.curr = get_tok(db, line, &sx.i, q);
	}
	return (SUCCESS);
}
