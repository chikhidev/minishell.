#include "./index.h"
#include "main.h"
#include "parsing.h"

/**
 * @param cut: the indexes of the start and the end to put the value
 */
int	update_index(t_db *db, char **line, char *var, t_str_cut *cut)
{
	char	*value;
	char	*insert;
	char	*left_section;
	char	*right_section;
	char	*tmp;
	int		updated_index;

	// echo " $FO  $HOME $ROOT "
	left_section = ft_substr(db, *line, 0, cut->start_ignore);
	updated_index = cut->start_ignore;
	if (var != NULL)
	{
		cut->end_ignore += ft_strlen(var);
		value = get_env(db, var);
		if (value != NULL)
			tmp = ft_strjoin(db, left_section, value);
		updated_index = cut->start_ignore + ft_strlen(value) - 1;
	}
	else if (cut->start_include != -1)
	{
		insert = ft_substr(db, *line, cut->start_include, cut->end_include
				- cut->start_include + 1);
		tmp = ft_strjoin(db, left_section, insert);
	}
	right_section = ft_substr(db, *line, cut->end_ignore, ft_strlen(*line)
			- cut->end_ignore);
	*line = ft_strjoin(db, tmp, right_section);
	return (updated_index);
}

int	updated_line(t_db *db, char **line, char *var, t_str_cut *cut)
{
	if (ft_strlen(var) == 0)
		return (cut->end_ignore);
	return (update_index(db, line, var, cut));
}
