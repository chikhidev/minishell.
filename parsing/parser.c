#include "main.h"
#include "parsing.h"
#include "builtens.h"

int parser(t_db *db, char *line)
{
    t_quote     *quotes;
    int         i;

    quotes = NULL;
    i = 0;
    if (ft_strlen(line) == 0)
        return (SUCCESS);
    skip_spaces(line, &i);

    if (line[i] == '\0')
        return (SUCCESS);

   
    if (track_quotes(db, &quotes, line) == FAILURE)
        return FAILURE;

    for (t_quote *q = quotes; q; q = q->next)
    {
        printf("quote: %c, start: %d, end: %d\n", q->ascii, q->start, q->end);
    }

    CATCH_ONFAILURE(
        syntax_checker(db, line, quotes),
        FAILURE
    )

    CATCH_ONFAILURE(
        smart_split(db, line, &db->root_node, NULL),
        FAILURE
    )
    return (SUCCESS);
}
