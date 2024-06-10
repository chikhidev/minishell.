#include "../includes/main.h"
#include "../includes/parsing.h"

// the head of the paranthesis linked list is 
int create_paranth(t_db *db, t_parnth **head, int open_)
{
    t_parnth *new;
    t_parnth *tmp;

    new = gc_malloc(db, sizeof(t_parnth));
    if (!new) return (FAILURE);
    new->open_ = open_;
    new->close_ = -1;
    new->next = NULL;
    if (!*head)
    {
        *head = new;
        return (SUCCESS);
    }
    tmp = *head;
    while (tmp->next)
        tmp = tmp->next;

    tmp->next = new;
    return (SUCCESS);
}

// {
//     t_parnth *new;
//     t_parnth *tmp;

//     new = gc_malloc(db, sizeof(t_parnth));
//     if (!new) return (FAILURE);
//     new->open_ = open_;
//     new->close_ = -1;
//     new->next = NULL;
//     if (!db->paranthesis)
//     {
//         db->paranthesis = new;
//         return (SUCCESS);
//     }
//     tmp = db->paranthesis;
//     while (tmp->next)
//         tmp = tmp->next;

//     tmp->next = new;
//     return (SUCCESS);
// }

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

int is_operator2(char    *s, int  i)
{
    printf("received %c  %d\n", s[i], i);
    if (s[i] == '>' && s[i + 1] == '>')
        return (true);
    if (s[i] == '<' && s[i + 1] == '<')
        return (true);
    if (s[i] == '&' && s[i + 1] == '&')
        return (true);
    if (s[i] == '|' && s[i + 1] == '|')
        return (true);
    if (s[i] == '<')
        return (true);
    if (s[i] == '>')
        return (true);
    if (s[i] == '|')
        return (true);
    return false;
}

t_parnth *get_parenth(t_db    *db, int    open_i)
{
    t_parnth    *scope;
    scope = db->paranthesis;

    while (scope)
    {
        if (scope->open_ == open_i)
            return scope;
        scope = scope->next;
    }
    return (NULL);
}

int verify_double_scope(t_db *db, char    *line)
{
    int         i;
    t_parnth    *scope;

    scope = db->paranthesis;
    while (scope)
    {
        i = scope->open_;
        while (line[i] && ++i < scope->close_)
        {
            if (line[i] == '(')
                return (FAILURE);
        }
        scope = scope->next;
    }
    return (SUCCESS);
}

t_parnth    *get_last_parenth(t_db  *db)
{
    t_parnth    *scope;

    scope = db->paranthesis;
    while (scope && scope->next)
        scope = scope->next;
    return scope;
}

int verify_scope_before(char   *line,  int scope_open_i, bool   is_first)
{
    int i;
    bool found_smtg;

    found_smtg = false;
    i = scope_open_i - 1;
    while (i >= 0)
    {
        while (i >= 0 && is_whitespace(line[i]))
            i--;
        if (i == -1 && is_first)
            return (SUCCESS);
        if (i >= 0 && is_operator(line, i))
            return (SUCCESS);
        else
            return FAILURE;
        i--;
    }
    if (found_smtg)
        return (FAILURE);
    return (SUCCESS);
}

int verify_scope_after(char   *line,  int scope_close_i, bool is_last)
{
    int i;
    bool found_smtg;

    found_smtg = false;
    i = scope_close_i + 1;
    while (line[i])
    {
        while (line[i] && is_whitespace(line[i]))
            i++;
        if (line[i] == '\0' && is_last)
            return (SUCCESS);
        if (is_operator2(line, i))
            return (SUCCESS);
        else
            return FAILURE;
        i++;
    }
    if (found_smtg)
        return (FAILURE);
    return (SUCCESS);
}

/* shouldnt have something other than op after and before scopes,
    or nothing for first scope before and last after
*/

int get_scopes_count(t_db *db)
{
    t_parnth    *scope;
    int         count;

    count = 0;
    scope = db->paranthesis;
    while (scope)
    {
        count++;
        scope = scope->next;
    }
    return count;
}


int verify_scope_surrounding(t_db  *db, char   *line)
{
    int scopes_count;
    int res;
    t_parnth    *scope;

    scopes_count = get_scopes_count(db);
    if (scopes_count == 0)
        return (SUCCESS);
    scope = db->paranthesis;
    // handle first (should have nothing before)
    res = verify_scope_before(line, scope->open_, true);
    if (!res)
        return (FAILURE);
    // handle other scopes
    while (scope)
    {
        res = verify_scope_after(line, scope->close_, (scope->next == NULL));
        if (!res)
            return (FAILURE);
        scope = scope->next;
    }
    return (SUCCESS);
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
        if (is_operator(line, idx))
            return (SUCCESS);
        if (line[idx] == '(')
            return (SUCCESS);
        if (line[idx] == ')')
            return (FAILURE);
        return (FAILURE);
    }
    return (FAILURE);
        
}

int track_paranthesis(t_db *db, t_parnth **head, char *line)
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
                return (error(db, "syntax error '('"));
            if (create_paranth(db, head, i) == FAILURE)
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
    if (verify_double_scope(db, line) == FAILURE)
        return error(db, "syntax error near '('");
    if (verify_scope_surrounding(db, line) == FAILURE)
        return error(db, "last syntax error");
    printf("1\n");
    return (SUCCESS);
}



