#include "main.h"
#include "parsing.h"
#include "builtens.h"

int parser(t_db *db, char *line)
{
    t_parnth    *paranthesis;
    t_quote     *quotes;
    int         i;

    paranthesis = NULL;
    quotes = NULL;
    i = 0;
    if (ft_strlen(line) == 0)
        return (SUCCESS);
    skip_spaces(line, &i);
    if (line[i] == '\0')
        return (SUCCESS);

    CATCH_ONFAILURE(
        track_quotes(db, &quotes, line),
        FAILURE
    )
    CATCH_ONFAILURE(
        syntax_checker(db, line, &i),
        FAILURE
    )
    
    CATCH_ONFAILURE(
        track_paranthesis(db, &paranthesis, line, quotes),
        FAILURE
    )
    CATCH_ONFAILURE(
        track_operators(db, line, quotes),
        FAILURE
    )

    CATCH_ONFAILURE(
        smart_split(db, line, &db->root_node, NULL),
        FAILURE
    )
    return (SUCCESS);
}
