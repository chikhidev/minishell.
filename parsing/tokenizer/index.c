/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   index.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abchikhi <abchikhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 02:48:50 by abchikhi          #+#    #+#             */
/*   Updated: 2024/09/14 01:51:23 by abchikhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"
#include "parsing.h"
#include "string.h"

int	handle_file(t_tokenizer *self)
{
	self->it.i += 1 + (this()->curr_type == HEREDOC
			|| this()->curr_type == APPENDFILE);
	skip_open_spaces(*self->quotes, self->line, &self->it.i);
	self->it.j = self->it.i;
	while (self->line[self->it.j] && !(is_whitespace(self->line[self->it.j])
			&& !is_inside_quotes_list(*self->quotes, self->it.j))
		&& !(validate_io(&self->line[self->it.j], 1) != INVALID
			|| validate_io(&self->line[self->it.j], 2) != INVALID))
		self->it.j++;
	self->save = ft_substr(this(), self->line, self->it.i, self->it.j
			- self->it.i);
	if (this()->curr_type == HEREDOC)
	{
		if (open_heredoc(this(), self->save) == FAILURE)
		{
			this()->exec_line = false;
			return (FAILURE);
		}
	}
	else if (open_file(this(), self->save, this()->curr_type) == FAILURE)
		self->read_write_perm = false;
	self->save = NULL;
	self->it.i = self->it.j - 1;
	return (SUCCESS);
}

int	saver(t_db *db, t_tokenizer *self)
{
	db->curr_type = validate_io(&self->line[self->it.i], 2);
	if (db->curr_type == INVALID)
		db->curr_type = validate_io(&self->line[self->it.i], 1);
	if (db->curr_type != INVALID)
	{
		if (handle_file(self) == FAILURE)
			return (FAILURE);
	}
	self->result = append_word(db, self->result, self->save);
	if (!self->result)
		return (FAILURE);
	self->save = NULL;
	return (SUCCESS);
}

int	token(t_tokenizer *self)
{
	bool	is_open_whitespace_;
	bool	is_open_io_;

	is_open_whitespace_ = is_open_whitespace(self->line, self->it.i,
			*self->quotes);
	is_open_io_ = is_open_io(self->line, self->it.i, *self->quotes);
	if (!is_open_whitespace_ && !is_open_io_ && self->read_write_perm)
	{
		self->save = concat(this(), self->save, self->line[self->it.i]);
	}
	else if (self->read_write_perm)
	{
		if (self->save && !is_inside_quotes_list(*self->quotes, self->it.i)
			&& contains(self->save, "*"))
		{
			handle_wildcard(this(), &self->result, self->save);
			self->save = NULL;
		}
		else if (saver(this(), self) == FAILURE)
			return (FAILURE);
	}
	return (SUCCESS);
}

char	**tokenize(t_db *db, t_quote **quotes, char *s)
{
	t_tokenizer	self;
	int			len;

	ft_bzero(&self, sizeof(t_tokenizer));
	self.quotes = quotes;
	self.line = s;
	self.result = (char **)gc_malloc(db, sizeof(char *));
	self.read_write_perm = true;
	self.save = NULL;
	skip_open_spaces(*self.quotes, self.line, &self.it.i);
	len = ft_strlen(self.line);
	while (self.it.i < len)
	{
		if (token(&self) == FAILURE)
			return (NULL);
		self.it.i++;
	}
	if (self.save && !is_inside_quotes_list(*quotes, self.it.i)
		&& contains(self.save, "*"))
		handle_wildcard(db, &self.result, self.save);
	else if (self.save && saver(db, &self) == FAILURE)
		return (NULL);
	return (self.result);
}
