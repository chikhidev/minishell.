#include "main.h"
#include "parsing.h"
#include "string.h"

int get_tok(t_db *db, char *s, int *i, t_quote *quotes)
{
    (void) quotes;
    if (s[*i] == '\0')
        return INVALID;

    if (is_whitespace(s[*i]))
        return W_SPACE;

    if (s[*i] == '|')
        return PIPE;

    if (s[*i] == '<')
    {
        if (s[*i + 1] == '<')
        {
            (*i)++;
            db->heredoc_counter++;
            return HEREDOC;
        }
        return INPUT;
    }
    if (s[*i] == '>')
    {
        if (s[*i + 1] == '>')
        {
            (*i)++;
            return APPEND;
        }
        return REDIR;
    }
    return WORD;
}
