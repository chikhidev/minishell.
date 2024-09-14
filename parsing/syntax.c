/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   syntax.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/14 17:35:07 by sgouzi            #+#    #+#             */
/*   Updated: 2024/09/14 23:09:12 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"

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

void	init_sx(t_syntax *sx, t_quote *quotes)
{
	sx->curr = INVALID;
	sx->i = 0;
	sx->found_word = false;
	sx->cont = false;
	sx->hd_msg = "maximum here-document count exceeded";
	sx->quotes = quotes;
	sx->next_tok = 0;
	sx->msg = NULL;
}

int	handle_pipe_syntax(t_syntax *sx, t_quote *quotes, char *line)
{
	t_db	*db;

	db = this();
	sx->msg = ft_strjoin(db, "syntax error near unexpected token `",
			get_token_error(db, sx->curr, sx->next_tok));
	sx->msg = ft_strjoin(db, sx->msg, "'");
	if (!sx->found_word)
		return (error(db, "syntax", sx->msg));
	if (sx->next_tok == INVALID)
		return (error(db, "syntax", sx->msg));
	if (sx->next_tok == PIPE)
		return (error(db, "syntax", sx->msg));
	skip_word(db, quotes, line, &sx->i);
	sx->found_word = true;
	sx->cont = true;
	return (SUCCESS);
}

int	handle_syntax(t_syntax *sx, t_quote *quotes, char *line)
{
	t_db	*db;

	db = this();
	if (db->heredoc_counter > 16)
		return (error(db, "heredoc", sx->hd_msg));
	sx->next_tok = get_next_tok(quotes, line, &sx->i);
	if (sx->curr == PIPE)
	{
		if (handle_pipe_syntax(sx, quotes, line) == FAILURE)
			return (FAILURE);
	}
	else if (is_redir(sx->curr))
	{
		sx->msg = ft_strjoin(db, "syntax error near unexpected token `",
				get_token_error(db, sx->curr, sx->next_tok));
		sx->msg = ft_strjoin(db, sx->msg, "'");
		if (sx->next_tok != WORD)
			return (error(db, "syntax", sx->msg));
		skip_word(db, quotes, line, &sx->i);
		sx->found_word = true;
		sx->cont = true;
	}
	return (SUCCESS);
}

int	syntax_checker(t_db *db, char *line, t_quote *quotes)
{
	t_syntax	sx;

	init_sx(&sx, quotes);
	skip_open_spaces(quotes, line, &sx.i);
	while (line[sx.i])
	{
		sx.cont = false;
		sx.curr = get_tok(db, line, &sx.i, quotes);
		if (sx.curr == W_SPACE && line[sx.i] && is_whitespace(line[sx.i])
			&& ++sx.i)
			continue ;
		if ((sx.curr == WORD || sx.curr == INVALID))
		{
			skip_word(db, quotes, line, &sx.i);
			sx.found_word = true;
			continue ;
		}
		else if (sx.curr != WORD && sx.curr != INVALID)
			if (handle_syntax(&sx, quotes, line) == FAILURE)
				return (FAILURE);
		if (sx.cont)
			continue ;
		++sx.i;
	}
	return (SUCCESS);
}
