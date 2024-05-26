#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function will split the line into commands and operators
 * depending on the priority of the operators
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

// int track_paranthesis(t_db *db, char *line)
// {
    
//     return (SUCCESS);
// }

int track_quotes(t_db *db, char *line)
{
    int i;

    if (!db->quotes)
    {
        db->quotes = gc_malloc(db, sizeof(t_quote*));
        if (!db->quotes) return (FAILURE);
        db->quotes
    }
    return (SUCCESS);
}

/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/
int parser(t_db *db, char *line)
{
    if (track_quotes(db, line) == FAILURE) return (FAILURE);
    // if (track_paranthesis(db, line) == FAILURE) return (FAILURE);
    if (smart_split(db, line) == FAILURE) return (FAILURE);
    return (SUCCESS);
}
