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
    if (s[*i] == SNGLQUOTE || s[*i] == DBL_QUOTE)
    {
        
    }
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

static int	is_space(char c)
{
	return ((c <= 13 && c >= 9) || c == 32);
}

void	skip_atoll(char *str, int *i, int *sign)
{
	while (is_space(str[(*i)]))
		(*i)++;
	if (str[(*i)] == '-' || str[(*i)] == '+')
	{
		if (str[(*i)] == '-')
			(*sign) = -1;
		(*i)++;
	}
}

long long	ft_atoll(char *str, bool *error)
{
	long long	res;
	int			sign;
	int			i;

	sign = 1;
	i = 0;
	res = 0;
	*error = false;
	skip_atoll(str, &i, &sign);
	while (str[i] >= '0' && str[i] <= '9')
	{
		if (res * sign > (LLONG_MAX - (str[i] - '0')) / 10 || res
			* sign < (LLONG_MIN + (str[i] - '0')) / 10)
		{
			*error = true;
			return (0);
		}
		res = res * 10 + (str[i] - '0');
		i++;
	}
	skip_spaces((char *)str, &i);
	if (str[i] != '\0')
		*error = true;
	return (res * sign);
}
