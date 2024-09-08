#include "main.h"
#include "parsing.h"

char	*gc_join(t_db *db, char *s1, char *s2)
{
	char	*new;
	int		i;
	int		j;

	if (!s1 && !s2)
		return (NULL);
	new = gc_malloc(db, ft_strlen(s1) + ft_strlen(s2) + 1);
	if (!new)
		return (NULL);
	i = 0;
	j = 0;
	while (s1 && s1[i])
	{
		new[j] = s1[i];
		i++;
		j++;
	}
	i = 0;
	while (s2 && s2[i])
	{
		new[j] = s2[i];
		i++;
		j++;
	}
	new[j] = '\0';
	return (new);
}

bool	contains_spaces_btwn(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (is_whitespace(s[i]))
		{
			skip_spaces(s, &i);
			if (!s[i])
				return (false);
			if (s[i] && !is_whitespace(s[i]))
				return (true);
		}
		i++;
	}
	return (false);
}

char	*ft_strdup_ec(t_db *db, const char *s1)
{
	const char	*clone;
	int			len;

	if (s1 == NULL)
		return (NULL);
	len = ft_strlen(s1);
	clone = ec_malloc(db, sizeof(char) * (len + 1));
	if (!clone)
		return (NULL);
	ft_memcpy((void *)clone, s1, len + 1);
	return ((char *)clone);
}

char	*ft_strjoin_ec(t_db *db, char const *s1, char const *s2)
{
	int		len1;
	int		len2;
	char	*res;

	if (!s1 && !s2)
		return (NULL);
	else if (!s1 && s2)
		return (ft_strdup_ec(db, s2));
	else if (s1 && !s2)
		return (ft_strdup_ec(db, s1));
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	res = ec_malloc(db, sizeof(char) * (len1 + len2 + 1));
	if (!res)
		return (NULL);
	ft_memcpy(res, s1, len1);
	ft_memcpy(res + len1, s2, len2);
	res[len1 + len2] = '\0';
	return (res);
}

int	is_str_empty(t_db *db, char *str)
{
	char	*unquoted;

	unquoted = without_quotes(db, str, NULL);
	if (ft_strlen(unquoted) == 0)
		return (1);
	return (0);
}

bool	starts_with(char *str, char *sub)
{
	return (ft_strncmp(str, sub, ft_strlen(sub)) == 0);
}