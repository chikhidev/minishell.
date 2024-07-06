#include "../includes/main.h"
#include "../includes/parsing.h"

// the head of the paranthesis linked list is 
int create_paranth(t_db *db, t_parnth **head, int open_)
{
    t_parnth *new;
    t_parnth *tmp;

    new = gc_malloc(db, sizeof(t_parnth));
    if (!new) return (error(db, "Malloc failed"));
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

t_parnth *last_unclosed_paranth(t_parnth *head)
{
    t_parnth *tmp;
    t_parnth *last;

    last = NULL;
    tmp = head;
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

t_parnth *get_parenth(t_parnth  *parenth, int    open_i)
{
    t_parnth    *scope;
    scope = parenth;

    while (scope)
    {
        if (scope->open_ == open_i)
            return scope;
        scope = scope->next;
    }
    return (NULL);
}

int verify_double_scope(t_parnth    *parenth, char    *line)
{
    int         i;
    t_parnth    *open_parenth1;
    t_parnth    *open_parenth2;

    i = 0;
    while (line[i])
    {
        if (line[i] == '(' && line[i + 1] == '(')
        {
            open_parenth1 = get_parenth(parenth, i);
            if (!open_parenth1)
                return (FAILURE);
            open_parenth2 = get_parenth(parenth, i + 1);
            if (!open_parenth2)
                return (FAILURE);
            if (open_parenth2->close_ + 1 == open_parenth1->close_)
                return (FAILURE);
        }
        i++;
    }
    return (SUCCESS);
}

t_parnth    *get_last_parenth(t_parnth *parenth)
{
    t_parnth    *scope;

    scope = parenth;
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
        if (i >= 0 && line[i] == '(')
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

int verify_scope_after(char   *line,  int scope_close_i)
{
    int i;
    bool found_smtg;

    found_smtg = false;
    i = scope_close_i + 1;
    while (line[i])
    {
        while (line[i] && is_whitespace(line[i]))
            i++;
        if (line[i] == '\0')
            return (SUCCESS);
        if (line[i] == ')')
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

int verify_scope_surrounding(t_parnth  *parenth, char   *line)
{
    t_parnth *first;
    t_parnth *last;
    t_parnth *curr;

    first = parenth;
    last = get_last_parenth(parenth);
    if (!first || !last)
        return (SUCCESS);
    if (verify_scope_before(line, first->open_, true) == FAILURE)
        return (FAILURE);
    if (verify_scope_after(line, first->close_) == FAILURE)
        return (FAILURE);
    curr = first->next;
    while (curr)
    {
        if (verify_scope_before(line, curr->open_, false) == FAILURE)
            return (FAILURE);
        if (verify_scope_after(line, curr->close_) == FAILURE)
            return (FAILURE);
        curr = curr->next;
    }
    return (SUCCESS);
}

int verify_create_parenth(t_parnth *head, char *line, int idx)
{
    // no prev scopes mean this is first parenthesy so its good
    if (!head)
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

int only_spaces(char *line, int start, int close)
{
    int i;

    i = start + 1;
    while (i < close)
    {
        if (is_whitespace(line[i]))
            return (FALSE);
        i++;
    }
    return (TRUE);
}

// db->paranthesis
int track_paranthesis(t_db *db, t_parnth **head, char *line, t_quote *quotes)
{
    t_parnth    *last_opened;
    int         i;

    i = 0;
    while (line[i])
    {
        last_opened = last_unclosed_paranth(*head);
        if (line[i] == '(' && !is_inside_quotes(quotes, i))
        {
            if (verify_create_parenth(*head, line, i) == FAILURE)
                return (error(db, "syntax error '('"));
            if (create_paranth(db, head, i) == FAILURE)
                return (FAILURE);
        }
        else if (line[i] == ')'
            && !is_inside_quotes(quotes, i)
            && last_opened
            && only_spaces(line, last_opened->open_, last_opened->close_))
            return error(db, "syntax error: near ')'");
        else if (line[i] == ')'
            && !is_inside_quotes(quotes, i)
            && last_opened)
        {
            if (!*head)
                return error(db, "syntax error: near ')'");
            last_opened->close_ = i;
        }
        else if (line[i] == ')' && !is_inside_quotes(quotes, i))
            return error(db, "syntax error: near ')'");
        i++;
    }
    if (last_unclosed_paranth(*head))
        return error(db, "syntax error: near '('");
    if (verify_double_scope(*head, line) == FAILURE)
        return FAILURE;
    if (verify_scope_surrounding(*head, line) == FAILURE)
        return error(db, "syntax error: near '(' or ')'");
    return (SUCCESS);
}

