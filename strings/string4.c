#include "main.h"
#include "parsing.h"

int	find_c(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (i);
		i++;
	}
	return (-1);
}

int	length_til(char *str, char stop)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == stop)
			return (i);
		i++;
	}
	return (-1);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	if (!s1 || !s2)
		return (-1);
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(const unsigned char *)s1 - *(const unsigned char *)s2);
}

bool	is_newline_at_the_end(char *line)
{
	int	i;

	i = 0;
	while (line[i])
	{
		i++;
	}
	return (i > 0 && line[i - 1] == '\n');
}

char	*gc_copy(t_db *db, char *s)
{
	char *new;
	int i;

	if (!s)
		return (NULL);
	new = gc_malloc(db, ft_strlen(s) + 1);
	if (!new)
		return (NULL);
	i = 0;
	while (s[i])
	{
		new[i] = s[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}