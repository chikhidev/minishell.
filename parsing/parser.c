#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function's job is to find the biggest operator in the line
 * - if there are paranthesis, it should ignore the operators inside the paranthesis
 * @return the biggest operator
*/
void    *biggest_op(t_db *db, char *line)
{
    if (db->paranthesis)
    {
        // find the biggest operator in case of paranthesis
    }
    else
    {
        // find the biggest operator by finding the last one in case of (&& ||)
        // if there are no && or ||, prioritize the pipe
    }
}

/**
 * @details This function should:
 * 1 - split scopes with paranthesis if available
 * 2 - split using whitespaces and quotes
 * @return 1 on success, 0 on failure
*/
int smart_split(t_db *db, char *line)
{
    int i;
    void    *biggest_op;

    (void)db;
    biggest_op = NULL;
    i = 0;
    skip_spaces(line, &i);
    //decide the main deviser here
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

    // DEBUG --------------------------------------------------------
    printf(CYAN"\n[DEBUG] available scopes:\n"RESET);
    for (t_parnth *tmp = db->paranthesis; tmp; tmp = tmp->next)
    {
        for (int j = tmp->open_; j <= tmp->close_; j++)
            printf("%c", line[j]);
        printf("\n");
    }
    printf(BLUE"\n[DEBUG] quotes:\n"RESET);
    for (t_quote *tmp = db->quotes; tmp; tmp = tmp->next)
    {
        for (int j = tmp->start; j <= tmp->end; j++)
            printf("%c", line[j]);
        printf("\n");
    }
    printf("\n");
    // DEBUG --------------------------------------------------------

    if (smart_split(db, line) == FAILURE) return (FAILURE);
    return (SUCCESS);
}
