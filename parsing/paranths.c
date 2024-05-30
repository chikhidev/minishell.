#include "../includes/main.h"
#include "../includes/parsing.h"

int create_paranth(t_db *db, int open_)
{
    t_parnth *new;
    t_parnth *tmp;

    new = gc_malloc(db, sizeof(t_parnth));
    if (!new) return (FAILURE);
    new->open_ = open_;
    new->close_ = -1;
    new->next = NULL;
    if (!db->paranthesis)
    {
        db->paranthesis = new;
        return (SUCCESS);
    }
    tmp = db->paranthesis;
    while (tmp->next)
        tmp = tmp->next;

    tmp->next = new;
    return (SUCCESS);
}

t_parnth *last_unclosed_paranth(t_db *db)
{
    t_parnth *tmp;
    t_parnth *last;

    last = NULL;
    tmp = db->paranthesis;
    while (tmp)
    {
        if (tmp->close_ == -1)
            last = tmp;
        tmp = tmp->next;
    }
    return (last);
}

// a good parenthesy is one created first time or afte some operator

int is_operator(char    *s, int  i)
{
    if (s[i] == '>' && i > 0 && s[i - 1] == '>')
        return (true);
    if (s[i] == '<' && i > 0 && s[i - 1] == '<')
        return (true);
    if (s[i] == '&' && i > 0 && s[i - 1] == '&')
        return (true);
    if (s[i] == '|' && i > 0 && s[i - 1] == '|')
        return (true);
    if (s[i] == '<')
        return (true);
    if (s[i] == '>')
        return (true);
    if (s[i] == '|')
        return (true);
    return false;
}

int verify_create_parenth(t_db  *db, char   *line, int    idx)
{
    t_parnth    *scope;

    scope = db->paranthesis;

    // no prev scopes mean this is first parenthesy so its good
    if (!scope)
        return (SUCCESS);
    // we go back in reverse to check what is before parenth
    idx--;
    while (idx >= 0)
    {
        while (is_whitespace(line[idx]))
            idx--;
        if (line[idx] == ')')
            return (FAILURE);
        if (is_operator(line, idx))
            return (SUCCESS);
        return (FAILURE);
    }
    return (FAILURE);
        
}

int track_paranthesis(t_db *db, char *line)
{
    t_parnth    *last_opened;
    int         i;

    i = 0;
    while (line[i])
    {
        last_opened = last_unclosed_paranth(db);
        if (line[i] == '(' && !is_inside_quotes(db, i))
        {
            if (verify_create_parenth(db, line, i) == FAILURE)
                return (error(db, "syntax error"));
            if (create_paranth(db, i) == FAILURE)
                return (FAILURE);
        }
        else if (line[i] == ')'
            && !is_inside_quotes(db, i)
            && last_opened
            && last_opened->open_ == i - 1)
            return error(db, "syntax error: empty paranthesis");
        else if (line[i] == ')'
            && !is_inside_quotes(db, i)
            && last_opened)
        {
            if (!db->paranthesis)
                return error(db, "syntax error: no paranthesis to close");
            last_opened->close_ = i;
        }
        else if (line[i] == ')' && !is_inside_quotes(db, i))
            return error(db, "syntax error: no paranthesis to close");
        i++;
    }
    if (last_unclosed_paranth(db))
        return error(db, "syntax error: some paranthesis are not closed");
    return (SUCCESS);
}
