#include "exec.h"
#include "main.h"
#include "parsing.h"

bool	is_absolute_path(char *path)
{
	int	i;

	i = 0;
	skip_spaces(path, &i);
	if (path[i] == '/')
		return (true);
	return (false);
}

bool	is_relative_path(char *path)
{
	int	i;

	i = 0;
	skip_spaces(path, &i);
	if (path[i] && i < ((int)ft_strlen(path) - 1) && path[i] == '.' && path[i
		+ 1] == '/')
		return (true);
	return (false);
}
