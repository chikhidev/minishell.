#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function should:
 * 1 - split scopes with paranthesis if available
 * 2 - split using whitespaces and quotes
 * @return 1 on success, 0 on failure
*/
int smart_split(t_db *db, char *line)
{
    int i;

    (void)db;
    i = 0;
    skip_spaces(line, &i);
    return (SUCCESS);
}

/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/
int parser(t_db *db, char *line)
{
    if (track_quotes(db, line) == FAILURE) return (FAILURE);
    if (track_paranthesis(db, line) == FAILURE) return (FAILURE);
    if (smart_split(db, line) == FAILURE) return (FAILURE);
    return (SUCCESS);
}
