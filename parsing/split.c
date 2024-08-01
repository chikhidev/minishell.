#include "main.h"
#include "parsing.h"
#include "builtens.h"

int is_op2(char *line, int *i)
{
    if (line[*i] == '&' && line[*i + 1] && line[(*i) + 1] == '&')
        return AND;
    else if (line[*i] == '|' && line[*i + 1] && line[(*i) + 1] == '|')
        return (OR);
    else if (line[*i] == '|')
        return PIPE;
    return INVALID;
}

int is_op3(char *line, int *i)
{
    if (line[*i] == '&' && *i > 0 && line[(*i) - 1] == '&' && (*i)--)
        return AND;
    else if (line[*i] == '|' && *i > 0 && line[(*i) - 1] == '|' && (*i)--)
        return (OR);
    else if (line[*i] == '|')
        return PIPE;
    return INVALID;
}

void skip_op(int *i, char *line)
{
    if (line[*i] == '&' && line[*i + 1] && line[++(*i)] == '&')
        ((*i) += 2);
    else if (line[*i] == '|' && line[*i + 1] && line[(*i) + 1] == '|')
        ((*i) += 2);
    else if (line[*i] == '|')
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
        if (is_op2(line, &i) == op->op_presentation
            && !is_inside_quotes(tracker->quotes, i)
            && !is_inside_paranthesis(tracker->paranthesis, i))
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
    void **current_node;
    char        **splitted;
    int i;

    current_node = holder->current_node;
    CATCH_ONFAILURE(create_op_node(db, holder->op,
        holder->current_node, holder->parent), FAILURE);
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
        create_cmd_node(db, current_node, holder->parent) // create a command node -------<<<<<<<<
    , FAILURE);
    CURR_CMD->args = tokenize(db, &holder->tracker->quotes, line);
    if (db->error)
        return error(db, NULL, NULL);
    CATCH_MALLOC((CURR_CMD)->args);
    (CURR_CMD)->input_fd = db->input_fd;
    (CURR_CMD)->output_fd = db->output_fd;
    db->input_fd = STDIN_FILENO;
    db->output_fd = STDOUT_FILENO;
    if (is_built_in(CURR_CMD))
        return SUCCESS;
    for (int i = 0; (CURR_CMD)->args[i]; i++)
    {
        (CURR_CMD)->args[i] = whithout_quotes(db, (CURR_CMD)->args[i]);
        if ((CURR_CMD)->args[i] == NULL)
            return error(db, NULL, "Malloc failed");
    }
    return SUCCESS;
}

int process_scope(t_db *db, char *line, t_holder *holder)
{
    bool ignore;
    char *tmp;
    char *scope;
	t_iterators it;


    printf("treating line: %s\n", line);
    scope = NULL;
    ignore = false;
    ft_bzero(&it, sizeof(t_iterators));
    while (line[it.i])
    {
        if (((line[it.i] == OPEN_PARANTH || line[it.i] == CLOSE_PARANTH)
            && !is_inside_quotes(holder->tracker->quotes, it.i)))
            ignore = !ignore;
        else if (ignore)
        {
            tmp = concat(db, scope, line[it.i]);
            if (!tmp)
                return error(db, NULL, "Malloc failed");
            gc_free(db, scope);
            scope = tmp;
        }
        else if (!ignore)
        {
            db->curr_type = is_valid_op(line[it.i], 2);
            if (db->curr_type == INVALID)
                db->curr_type = is_valid_op(line[it.i], 1);
            if (db->curr_type != INVALID)
            {
                printf("was here: %d\n", it.i);
                it.i += 1 + (db->curr_type == HEREDOC || db->curr_type == APPEND);
                skip_open_spaces(holder->tracker->quotes, line, &it.i);
                printf("now here: %d\n", it.i);
                tmp = NULL;
                while (line[it.i] && !(is_whitespace(line[it.i]) && !is_inside_quotes(holder->tracker->quotes, it.i)))
                {
                    tmp = concat(db, tmp, line[it.i]);
                    it.i++;
                }
                printf("file name->>> %s\n", tmp);
                it.i -= 1;
            }
        }
        it.i++;
    }
    printf("scope: %s\n", scope);
    return SUCCESS;
    // return smart_split(db, scope, holder->current_node,
    //     holder->parent);
}

int smart_split(t_db *db, char *line, void **current_node, void *parent)
{
    t_holder holder;

    if (ft_strlen(line) == 0
        || all_whitespaces(line, 0, ft_strlen(line)))
        return SUCCESS;
    ft_bzero(&holder, sizeof(holder));
    holder.tracker = gc_malloc(db, sizeof(t_tracker));
    CATCH_MALLOC(holder.tracker);
    ft_bzero(holder.tracker, sizeof(t_tracker));
    CATCH_ONFAILURE(track_quotes(db, &holder.tracker->quotes, line), FAILURE);
    CATCH_ONFAILURE(track_paranthesis(db, &holder.tracker->paranthesis,
        line, holder.tracker->quotes), FAILURE);
    holder.op = strongest_operator(line, holder.tracker);
    holder.current_node = current_node;
    holder.parent = parent;
    if (holder.tracker->paranthesis && holder.op == NOT_FOUND)
    {
        return smart_split(db, 
            remove_paranthesis(db, line, holder.tracker->paranthesis)
        , current_node, parent);
        // return process_scope(db, line, &holder);
    }
    else if (holder.op != NOT_FOUND
        && process_op(db, line, &holder) == FAILURE)
            return FAILURE;
    else if (process_cmd(db, line, &holder) == FAILURE)
            return FAILURE;
    gc_free(db, holder.tracker);
    return SUCCESS; 
}
