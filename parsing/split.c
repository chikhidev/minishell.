#include "main.h"
#include "parsing.h"
#include "builtens.h"

void skip_op(int *i, char *line)
{
    if (line[*i] == '|')
        (*i)++;
    return;
}

char **split_line(t_db * db, char *line, t_op_node *op, t_tracker *tracker)
{
    int i;
    int len;
    int k;
    char **splitted;

    i = 0;
    len = 0;
    k = 0;

    splitted = gc_malloc(db, sizeof(char *) * op->n_childs);
    while (line[i])
    {
        if (is_op(line, &i) == op->op_presentation
            && !is_inside_quotes_list(tracker->quotes, i))
        {
            splitted[k] = gc_malloc(db, sizeof(char) * (len + 1));
            ft_strlcpy(splitted[k], line + i - len, len + 1);
            skip_op(&i, line);
            k++;
            len = 0;
        }
        len++;
        i++;
    }
    if (len > 0 && k < op->n_childs)
    {
        splitted[k] = gc_malloc(db, sizeof(char) * (len + 1));
        CATCH_ONNULL(splitted[k], NULL);
        ft_strlcpy(splitted[k], line + i - len, len + 1);
    }
    return splitted;
}

int process_op(t_db *db, char *line, t_holder *holder)
{
    void        **current_node;
    char        **splitted;
    int i;

    current_node = holder->current_node;
    CATCH_ONFAILURE(create_op_node(db, holder->op,
        holder->current_node), FAILURE);
    
    CURR_OP->n_childs = count_between_op(db, line,
        holder->op, holder->tracker);
    
    splitted = split_line(db, line, CURR_OP, holder->tracker);
    CATCH_MALLOC(splitted);

    CURR_OP->childs = gc_malloc(db, sizeof(void *) *
        CURR_OP->n_childs);
    
    ft_bzero(CURR_OP->childs, sizeof(void *) * CURR_OP->n_childs);

    i = 0;
    while (i < CURR_OP->n_childs)
    {
        CATCH_ONFAILURE(
            smart_split(db, splitted[i], &CURR_OP->childs[i], *current_node),
            FAILURE
        )
        i++;
    }
    return SUCCESS;
}

int process_cmd(t_db *db, char *line, t_holder *holder)
{
    void **current_node;

    current_node = holder->current_node;
    CATCH_ONFAILURE(
        create_cmd_node(db, current_node) // create a command node -------<<<<<<<<
    , FAILURE);

    if (expand(db, &line, &holder->tracker->quotes) == FAILURE)
        return FAILURE;

    CURR_CMD->args = tokenize(db, &holder->tracker->quotes, line);
    if (db->error || !db->exec_line)
        return error(db, NULL, NULL);

    CATCH_MALLOC((CURR_CMD)->args);
    (CURR_CMD)->input_fd = db->input_fd;
    (CURR_CMD)->output_fd = db->output_fd;
    db->input_fd = STDIN_FILENO;
    db->output_fd = STDOUT_FILENO;

    return SUCCESS;
}

/**
 * ---------------------------------------------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------
 * -------------------------------------------------------------------------------------------------------------------------------
 * -----------------------------------------------------------------------------------------------------------------
 * ---------------------------------------------------------------------------------------------------
 */


int smart_split(t_db *db, char *line, void **current_node, void *parent)
{
    t_holder holder; // holder varibale so we make sure of that this data will be given to children if exists

    if (db->error || !db->exec_line)
        return FAILURE;
    
    if (ft_strlen(line) == 0
        || all_whitespaces(line, 0, ft_strlen(line)))
        return SUCCESS;
    
    ft_bzero(&holder, sizeof(holder));
    holder.tracker = gc_malloc(db, sizeof(t_tracker));
    CATCH_MALLOC(holder.tracker);

    CATCH_ONFAILURE(track_quotes(db, &holder.tracker->quotes, line), FAILURE);
    
    holder.parent = parent;
    holder.op = strongest_operator(line, holder.tracker); 
    holder.current_node = current_node;
    /**
     * this case means we have an operator
     * take the most powerful operator and start splitting the line whenever that op is appeared
     * and if the operator is inside scope we assign the 'is_scope' to true 
     */
    if (holder.op != NOT_FOUND)
    {
        if (process_op(db, line, &holder) == FAILURE)
            return FAILURE;
    }
    /**
     *  in this case if we are left with no paranthesis and no operator this means that this must ne a COMMAND
     *  so we use a TOKENIZER to identify each character and use for each a spicific action
     */
    else
    {
        if (process_cmd(db, line, &holder) == FAILURE)
            return (db->error != true); // failure in case of error happened else just success
    }
    return SUCCESS;
}
