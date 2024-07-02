#include "../includes/main.h"
#include "../includes/parsing.h"

/*
    good = 1 means in good place (before and after)
*/
 
/* flag is -3 if needs something before op                 '<'    '|'                    */
/* flag is -2 if needs something after  op                 '>'    '<<'    '>>'           */
/* flag is -1 if needs something before & after op like    '&&'   '||'                   */

 
void set_up_flag(int    *flag, char *op)
{
    if (strcmp(op, "&&") == 0)
        *flag = -1;
    else if (strcmp(op, "||") == 0)
        *flag = -1;
    else if (strcmp(op, "|") == 0)
        *flag = -3;
    else if (strcmp(op, "<") == 0)
        *flag = -3;
    else if (strcmp(op, ">") == 0)
        *flag = -2;
    else if (strcmp(op, ">>") == 0)
        *flag = -2;
    else if (strcmp(op, "<<") == 0)
        *flag = -2;
    else
        error(NULL,"wrong op\n");
}

/* flag is -3 if needs something before op                 '<'    '|'                    */
/* flag is -2 if needs something after  op                 '>'    '<<'    '>>'           */
/* flag is -1 if needs something before & after op like    '&&'   '||'                   */


// ! TODO : op op is bad

int check_after_op( char    *line,   char    *op_name,   int op_idx,  int flag)
{
    int i;

    i = op_idx;
    i += ft_strlen(op_name);
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

int good_place_for_op( char    *line,   char    *op_name,   int op_idx,  int flag)
{
    int i;
    bool char_before_op;
    bool char_after_op;

    char_before_op = false;
    char_after_op = false;
    i = 0;
    (void) op_name;
    while (line[i])
    {
        // op_idx < i  means operator is before our caracter  ex: (< cat)
        if (ft_isalnum(line[i]) && i < op_idx)
        {
            if (flag == -3)
                return (SUCCESS);
            char_before_op = true;
        }
        // i >op_idx  means operator is after our caracter    ex: (ls |)
        if (ft_isalnum(line[i]) && i > op_idx)
        {
            if (flag == -2)
                return (SUCCESS);
            char_after_op = true;
        }
        i++;
    }
    if (char_before_op && char_after_op)
        return (SUCCESS);
    else
        return (FAILURE);
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
        while (tmp && tmp->next)
            tmp = tmp->next;
        tmp->next = op;
    }
    return (SUCCESS);
}

int track_operators(t_db *db, char  *line)
{
    t_operators *ops;
    int i;
    int flag;

    i = 0;
    while (line[i])
    {
        if (line[i] == '&' && line[i + 1] == '&')
            (create_operator(db, i, "&&"), i++);
        else if (line[i] == '|' && line[i + 1] == '|')
            (create_operator(db, i, "||"), i++);
        else if (line[i] == '<' && line[i + 1] == '<')
            (create_operator(db, i, "<<"), i++);
        else if (line[i] == '>' && line[i + 1] == '>')
            (create_operator(db, i, ">>"), i++);
        else if (line[i] == '<')
            create_operator(db, i, "<");
        else if (line[i] == '>')
            create_operator(db, i, ">");
        else if (line[i] == '|')
            create_operator(db, i, "|");

        i++;
    }
    i = 0;
    ops = db->ops;
    while (ops)
    {
        set_up_flag(&flag, ops->name);
        if (!good_place_for_op(line, ops->name, ops->i, flag))
            return error(db, "bad place for operator");
        if (!check_after_op(line, ops->name, ops->i, flag))
            return error(db, "bad place for op");
        // printf("op %s  i %d\n", ops->name, ops->i);

        // printf("operator -> %s   index -> %i\n", ops->name, ops->i);
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
        return (TRUE);
    else if (c == '|' && next_c && next_c == '|')
        return (TRUE);
    else if (c == '|' || c == '>' || c == '<')
        return (TRUE);
    return (FALSE);
}
