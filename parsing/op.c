#include "../includes/main.h"
#include "../includes/parsing.h"

/*
    good = 1 means in good place (before and after)
*/

/* flag is -3 if needs something before op                 '<', '|'                */
/* flag is -2 if needs something after  op                 '>' '<<' , '>>'         */
/* flag is -1 if needs something before & after op like    '&&' , '||'             */
void set_up_flag(int    *flag, char *op)
{
    if (strcmp(op, "&&") == 0)
        *flag = -1;
    else if (strcmp(op, "||") == 0)
        *flag = -1;
    else if (strcmp(op, "|") == 0)
        *flag = -2;
    else if (strcmp(op, ">") == 0)
        *flag = -2;
    else if (strcmp(op, ">>") == 0)
        *flag = -2;
    else if (strcmp(op, "<<") == 0)
        *flag = -2;
    else if (strcmp(op, "<") == 0)
        *flag = -3;
    else
        error(NULL,"wrong op\n");
}

int surrounded_by_parenths(t_db   *db,   int i)
{
    t_parnth    *parnth;
    int good;

    good = -1;
    parnth = db->paranthesis;
    while (parnth)
    {
        if (parnth->open_ < i && parnth->close_ < i)
            good++;
        if (parnth->open_ > i && parnth->close_ > i)
            good++;
        parnth = parnth->next;
    }
    if (good == 1)
        return (SUCCESS);
    else
        return (FAILURE);
}

/* flag is -3 if needs something before op                 '<'                      */
/* flag is -2 if needs something after  op                 '>' '<<' , '>>' '|'      */
/* flag is -1 if needs something before & after op like    '&&' , '||'              */

int good_place_for_op( char    *line,   char    *op_name,   int op_idx,  int flag)
{
    int i;
    bool seen_before;
    bool seen_after;

    seen_before = false;
    seen_after = false;
    i = 0;
    while (line[i])
    {
        if (isalnum(line[i]) && op_idx < i)
        {
            if (flag == -3)
                return (SUCCESS);
            seen_before = true;
        }
        if (isalnum(line[i]) && (op_idx + (int)ft_strlen(op_name)) > i)
        {
            if (flag == -2)
                return (SUCCESS);
            seen_after = true;
        }
        i++;
    }
    if (seen_before && seen_after)
        return (SUCCESS);
    else
        return (FAILURE);
}

int create_operator(t_db *db, int i, char *name)
{
    t_operators *op;
    t_operators *tmp;

    op = malloc(sizeof(t_operators));
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
        // printf("operator -> %s   index -> %i\n", ops->name, ops->i);
        ops = ops->next;
    }
    return (SUCCESS);
}

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
