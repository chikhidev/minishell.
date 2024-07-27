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

int smart_split(t_db *db, char *line, void **current_node, void *parent)
{
    t_tracker   *tracker;
    char        **splitted;
    int         op;
    int         i;

    if (ft_strlen(line) == 0)
        return SUCCESS;
    if (all_whitespaces(line, 0, ft_strlen(line)))
        return SUCCESS;

    tracker = gc_malloc(db, sizeof(t_tracker));
    CATCH_MALLOC(tracker);
    ft_bzero(tracker, sizeof(t_tracker));

    CATCH_ONFAILURE(track_quotes(db, &tracker->quotes, line), FAILURE);
    CATCH_ONFAILURE(track_paranthesis(db, &tracker->paranthesis,
        line, tracker->quotes), FAILURE);
    op = strongest_operator(line, tracker);

    if (tracker->paranthesis && op == NOT_FOUND)
    {
        return smart_split(db, remove_paranthesis(db, line, tracker->paranthesis), current_node, parent);
    }
    else if (op != NOT_FOUND)
    {
        CATCH_ONFAILURE(create_op_node(db, op, current_node, parent), FAILURE);
        // create the childs of the operator node <<<<<<<<
        CURR_OP->n_childs = count_between_op(db, line, op, tracker);
        splitted = split_line(db, line, CURR_OP, tracker);
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
    }
    else
    {
        // command scope <<<<<<<<
        CATCH_ONFAILURE(
            create_cmd_node(db, current_node, parent) // create a command node -------<<<<<<<<
        , FAILURE);
        

        ((t_cmd_node *)*current_node)->args = ft_new_split(db, tracker->quotes, line);
        if (db->error)
            return error(db, NULL, NULL);
        CATCH_MALLOC((CURR_CMD)->args);

        // set the redirections of the command node if any
        if (db->input_fd != INVALID)
            (CURR_CMD)->input_fd = db->input_fd;
        if (db->output_fd != INVALID)
            (CURR_CMD)->output_fd = db->output_fd;
        db->input_fd = INVALID;
        db->output_fd = INVALID;

        // expand each argument
        for (int i = 0; (CURR_CMD)->args[i]; i++)
        {
            track_quotes(db, &(tracker->quotes), CURR_CMD->args[i]);
            CATCH_ONFAILURE(
                expand(db, &(CURR_CMD)->args[i], tracker->quotes)
            , FAILURE);
            CURR_CMD->args[i] = whithout_quotes_free_db(db, CURR_CMD->args[i]);
            CATCH_ONNULL(
                CURR_CMD->args[i], error(db, NULL, "Malloc failed")
            )
        }

        // check for built-in(s)
        if (ft_strcmp(CURR_CMD->args[0], "echo") == 0)
            echo(CURR_CMD->args, 3);
        else if (ft_strcmp(CURR_CMD->args[0], "export") == 0)
            export(db, CURR_CMD->args);
        else if (ft_strcmp(CURR_CMD->args[0], "pwd") == 0)
            pwd(db);
        else if (ft_strcmp(CURR_CMD->args[0], "env") == 0)
            env(db);
        else if (ft_strcmp(CURR_CMD->args[0], "cd") == 0)
            cd(db, CURR_CMD->args);
        else if (ft_strcmp(CURR_CMD->args[0], "exit") == 0)
        {
            error(db, NULL, NULL);
            free_environment(db);
            exit(0);
        }
    }
    gc_free(db, tracker);
    return SUCCESS; 
}
