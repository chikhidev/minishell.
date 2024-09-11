#include "builtens.h"
#include "main.h"
#include "parsing.h"

int	parser(t_db *db, char *line)
{
	t_quote	*quotes;
	int		i;

	quotes = NULL;
	i = 0;
	if (ft_strlen(line) == 0)
		return (SUCCESS);
	skip_spaces(line, &i);
	if (line[i] == '\0')
		return (SUCCESS);
	if (track_quotes(db, &quotes, line) == FAILURE)
		return (FAILURE);
    if (syntax_checker(db, line, quotes) == FAILURE)
        return (FAILURE);
    if (smart_split(db, line, &db->root_node, NULL) == FAILURE)
        return (FAILURE);
    else
        return (SUCCESS);
}
