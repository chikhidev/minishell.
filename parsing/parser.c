#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function will split the line into commands and operators
 * depending on the priority of the operators
 * @return 1 on success, 0 on failure
*/
int smart_split(t_db *db, char *line)
{
    (void)line;
    return error(db, "smart_split not implemented");
    return (SUCCESS);
}

/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/
int parser(t_db *db, char *line)
{
    if (smart_split(db, line) == FAILURE) return (FAILURE);
    return (SUCCESS);
}
