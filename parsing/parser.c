#include "../includes/main.h"
#include "../includes/parsing.h"



/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/
int parser(t_db *db, char *line)
{
    int i;

    i = 0;
    if (ft_strlen(line) == 0) return (SUCCESS);
    skip_spaces(line, &i);
    if (line[i] == '\0') return (SUCCESS);
    if (track_quotes(db, line) == FAILURE) return (FAILURE);
    if (track_paranthesis(db, &db->paranthesis, line) == FAILURE) return (FAILURE);
    if (track_operators(db, line) == FAILURE) return (FAILURE);
    if (expand(db, &line) == FAILURE) return (FAILURE);
    if (smart_split(db, line) == FAILURE) return (FAILURE);

    // DEBUG --------------------------------------------------------
    printf(MAGENTA"\n[DEBUG] line: %s\n"RESET, line);

    // for (t_parnth *p = db->paranthesis; p; p = p->next)
    // {
    //     printf("close -> [%d]    open -> [%d]\n", p->close_, p->open_);
    //     printf("\n");
    // }
    return (SUCCESS);
}
