// #include "./index.h"
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
	updated_index = 0;
	if (var != NULL)
	{
		cut->end_ignore += ft_strlen(var);
		value = get_env(db, var);
		if (value != NULL)
		{
			tmp = ft_strjoin(db, left_section, value);
		}
	}
	else if (cut->start_include != -1)
	{
		insert = ft_substr(db, *line, cut->start_include, cut->end_include
				- cut->start_include + 1);
		tmp = ft_strjoin(db, left_section, insert);
		updated_index = cut->end_include - 2;
	}
	right_section = ft_substr(db, *line, cut->end_ignore, ft_strlen(*line)
			- cut->end_ignore);
	*line = ft_strjoin(db, tmp, right_section);
	// printf(RED"will return in: %d->%c\n"RESET, updated_index, (*line)[updated_index]);
	return (updated_index);
}

int	updated_line(t_db *db, char **line, char *var, t_str_cut *cut)
{

	// printf(GREEN"----------------------\n");
	// printf("line: [%s]\n", *line);
	// printf("var: [%s]\n", var);
	// printf("cut: [%d, %d, %d, %d]\n", cut->start_ignore, cut->end_ignore,
	// 	cut->start_include, cut->end_include);
	// printf("----------------------\n"RESET);

	if (ft_strlen(var) == 0)
		return (cut->end_ignore);
	return (update_index(db, line, var, cut));
}
