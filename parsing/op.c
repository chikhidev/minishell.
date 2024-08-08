#include "main.h"
#include "parsing.h"

/*
    good = 1 means in good place (befor and after)
*/
 
/* flag is -2 if needs something after  op                 '>'    '<<'    '>>'     '<'    '|'          */
/* flag is -1 if needs something befor & after op like    '&&'   '||'                                  */

 
void set_up_flag(int    *flag, char *op)
{
    if (ft_strcmp(op, "&&") == 0)
        *flag = -1;
    else if (ft_strcmp(op, "||") == 0)
        *flag = -1;
    else if (ft_strcmp(op, "|") == 0)
        *flag = -1;
    else if (ft_strcmp(op, "<") == 0)
        *flag = -2;
    else if (ft_strcmp(op, ">") == 0)
        *flag = -2;
    else if (ft_strcmp(op, ">>") == 0)
        *flag = -2;
    else if (ft_strcmp(op, "<<") == 0)
        *flag = -2;
    else
        error(NULL, NULL, "wrong op\n");
}

/* flag is -2 if needs something after  op                 '>'    '<<'    '>>'    '<'            */
/* flag is -1 if needs something befor & after op like    '&&'    '||'    '|'                    */


// ! TODO : op op is bad

int check_after_op( char    *line,   char    *op_name,   int op_idx,  int flag)
{
    int i;

    i = op_idx;
    i += ft_strlen(op_name);
    printf("checking for %s\n", op_name);
    if (flag == 3)
        return (SUCCESS);
    while(line[i])
    {
        if (is_whitespace(line[i]))
            i++;
        if (!isalnum(line[i]) && line[i] == '(' && line[i] == ')')
            return FAILURE;
        return SUCCESS;
    }
    return SUCCESS;
}

// && befor after 
// < 
bool check_befor( char    *line,   char    *op_name,   int op_idx)
{
    int i;
    (void)op_name;
    i = op_idx - 1;
    while (i >= 0)
    {
        if (line[i] == '(')
            return 0;
        else if (is_op3(line, &i) != INVALID)
            return 0;
        else if (is_whitespace(line[i]))
            i--;
        else
            return 1;
    }
    return (0);
}

bool check_after( char    *line,   char    *op_name,   int op_idx)
{
    int len;
    int i;

    len = ft_strlen(line);
    i = op_idx + ft_strlen(op_name);
    while (i < len)
    {
        if (line[i] == ')')
            return 0;
        if (is_op(line, &i) != INVALID)
            return 0;
        else if (is_whitespace(line[i]))
            i++;
        else
            return 1;
    }
    return (0);
}

int good_place_for_op( char    *line,   char    *op_name,   int op_idx,  int flag)
{
    bool char_after;
    bool char_befor;
    char_befor = false;
    char_after = false;
    if (check_befor(line, op_name, op_idx))
        char_befor = true;
    if (check_after(line, op_name, op_idx))
    {
        if (flag == -2)
            return 1;
        char_after = true;
    }
    if (flag == -1 && char_befor && char_after)
        return 1;
    return 0;
}

int create_operator(t_db *db, int i, char *name)
{
    t_operators *op;
    t_operators *tmp;

    op = gc_malloc(db, sizeof(t_operators));
    if (!op)
        return (FAILURE);
    op->i = i;
    op->name = name;
    op->size = ft_strlen(name);
    op->next = NULL;
    if (!db->ops)
    {
        op->next = db->ops;
        db->ops = op;
    }
    else
    {
        tmp = db->ops;

        while (tmp->next)
            tmp = tmp->next;
        tmp->next = op;
    }
    return (SUCCESS);
}
// int track_operators(t_db *db, char  *line, t_quote  *quotes)
// {
//     int i;
//     (void) db;
//     i = 0;
//     while (line[i])
//     {
//         if (line[i] == '&' && line[i + 1] == '&' && is_inside_quotes(quotes, i))
//             return (FAILURE);
//         else if (line[i] == '|' && line[i + 1] == '|' && is_inside_quotes(quotes, i))
//             return (FAILURE);
//         else if (line[i] == '|'  && is_inside_quotes(quotes, i))
//             return (FAILURE);
//         i++;
//     }
//     return (SUCCESS);
// }

int track_operators(t_db *db, char  *line, t_quote  *quotes)
{
    t_operators *ops;
    int i;
    int flag;

    i = 0;
    while (line[i])
    {
        if (line[i] == '&' && line[i + 1] == '&' && !is_inside_quotes(quotes, i))
            (create_operator(db, i, "&&"), i++);
        else if (line[i] == '|' && line[i + 1] == '|' && !is_inside_quotes(quotes, i))
            (create_operator(db, i, "||"), i++);
        else if (line[i] == '|' && !is_inside_quotes(quotes, i))
            create_operator(db, i, "|");
        i++;
    }
    i = 0;
    ops = db->ops;
    while (ops)
    {
        set_up_flag(&flag, ops->name);
        if (!good_place_for_op(line, ops->name, ops->i, flag))
            return error(db, NULL, "bad place for operator");
        ops = ops->next;
    }
    return (SUCCESS);
}

/* sould check if the op in within scopes */
// int is_op_trackable(t_db    *db, char   *op_name, int   op_idx)
// {
//     int trackable;
//     t_parnth    *scope;

//     trackable = -1;
//     scope = db->paranthesis;
//     while (scope)
//     {
//         if (scope->open_ < op_idx && scope->close_ < op_idx)
//             trackable++;
//     }
// }

int is_valid_op(char c, char next_c)
{
    if (c == '&' && next_c && next_c == '&')
        return (true);
    else if (c == '|' && next_c && next_c == '|')
        return (true);
    else if (c == '|' || c == '>' || c == '<')
        return (true);
    return (false);
}
