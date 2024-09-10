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
		return (PIPE);
	if (s[*i] == '<')
	{
		if (s[*i + 1] == '<')
		{
			(*i)++;
			db->heredoc_counter++;
			return (HEREDOC);
		}
		return (INPUT);
	}
	if (s[*i] == '>')
	{
		if (s[*i + 1] == '>')
		{
			(*i)++;
			return (APPEND);
		}
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
	t_db *db;

	db = this();
	while (*s)
	{
		ft_write(db, fd, s, 1);
		s++;
	}
}
