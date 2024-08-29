#include "main.h"
#include "parsing.h"

int is_special(t_db *db, char *s, int *i, t_quote *quotes)
{
    if (is_inside_quotes_list(quotes, *i))
        return INVALID;

    if (is_whitespace(s[*i]))
    {
        printf("is w space\n");
        return W_SPACE;
    }

    if (s[*i] == '|')
    {
        printf("is pipe\n");
        return PIPE;
    }

    if (s[*i] == '<')
    {
        if (s[*i + 1] == '<')
        {
            (*i)++;
            db->heredoc_counter++;
            printf("is hrdc\n");

            return HEREDOC;
        }
        printf("is input\n");

        return INPUT;
    }

    if (s[*i] == '>')
    {
        if (s[*i + 1] == '>')
        {
            (*i)++;
            printf("is append\n");

            return APPEND;
        }
        printf("is redir\n");

        return REDIR;
    }

    return INVALID;
}
