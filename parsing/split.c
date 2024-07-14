#include "../includes/main.h"
#include "../includes/parsing.h"

int is_op2(char *line, int *i)
{
    if (line[*i] == '&' && line[*i + 1] && line[(*i) + 1] == '&')
        return AND;
    else if (line[*i] == '|' && line[*i + 1] && line[(*i) + 1] == '|')
        return (OR);
    else if (line[*i] == '>' && line[*i + 1] && line[(*i) + 1] == '>')
        return (APPEND);
    else if (line[*i] == '<' && line[*i + 1] && line[(*i) + 1] == '<')
        return (HEREDOC);
    else if (line[*i] == '|')
        return PIPE;
    else if (line[*i] == '>')
        return REDIR;
    else if (line[*i] == '<')
        return INPUT;
    return INVALID;
}

int is_op3(char *line, int *i)
{
    if (line[*i] == '&' && *i > 0 && line[(*i) - 1] == '&' && (*i)--)
        return AND;
    else if (line[*i] == '|' && *i > 0 && line[(*i) - 1] == '|' && (*i)--)
        return (OR);
    else if (line[*i] == '>' && *i > 0 && line[(*i) - 1] == '>' && (*i)--)
        return (APPEND);
    else if (line[*i] == '<' && *i > 0 && line[(*i) - 1] == '<' && (*i)--)
        return (HEREDOC);
    else if (line[*i] == '|')
        return PIPE;
    else if (line[*i] == '>')
        return REDIR;
    else if (line[*i] == '<')
        return INPUT;
    return INVALID;
}

void skip_op(int *i, char *line)
{
    if (line[*i] == '&' && line[*i + 1] && line[++(*i)] == '&')
        ((*i) += 2);
    else if (line[*i] == '|' && line[*i + 1] && line[(*i) + 1] == '|')
        ((*i) += 2);
    else if (line[*i] == '>' && line[*i + 1] && line[(*i) + 1] == '>')
        ((*i) += 2);
    else if (line[*i] == '<' && line[*i + 1] && line[(*i) + 1] == '<')
        ((*i) += 2);
    else if (line[*i] == '|')
        (*i)++;
    else if (line[*i] == '>')
        (*i)++;
    else if (line[*i] == '<')
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

    CATCH_ERROR; // <<<<< catch golbal error
    if (ft_strlen(line) == 0) return SUCCESS;
    if (all_whitespaces(line, 0, ft_strlen(line))) return SUCCESS;


    tracker = gc_malloc(db, sizeof(t_tracker));
    if (!tracker) return error(db, "Failed to allocate memory");
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
        CURR_OP->neighbour = NULL;
        // create the childs of the operator node <<<<<<<<
        CURR_OP->n_childs = count_between_op(db, line, op, tracker);

        splitted = split_line(db, line, CURR_OP, tracker);
        CATCH_MALLOC(splitted);

        CATCH_ONNULL(splitted, FAILURE);
        
        CURR_OP->childs = gc_malloc(db, sizeof(void *) * 
            CURR_OP->n_childs);
        ft_bzero(CURR_OP->childs, sizeof(void *) * CURR_OP->n_childs);

        i = 0;
        while (i < CURR_OP->n_childs)
        {
            if (op == HEREDOC && i == 0 && !is_the_first(line, tracker, op))
            {
                CURR_OP->neighbour = gc_malloc(db, sizeof(t_cmd_node));
                CATCH_MALLOC(CURR_OP->neighbour);
                CATCH_ONFAILURE(
                    smart_split(db, splitted[i], &CURR_OP->neighbour, *current_node),
                    FAILURE
                )
            }
            else
            {
                CATCH_ONFAILURE(
                    smart_split(db, splitted[i], &CURR_OP->childs[i], *current_node),
                    FAILURE
                )
            }
            i++;
        }

    }
    else
    {
        CATCH_ONFAILURE(
            create_cmd_node(db, current_node, parent) // create a command node -------<<<<<<<<<<<<<<<
        , FAILURE);
        ((t_cmd_node *)*current_node)->args = ft_new_split(db, tracker->quotes, line);
        CATCH_MALLOC(((t_cmd_node *)*current_node)->args);
        // command scope <<<<<<<<

        // expand each argument
        // except in case of heredoc first argument
        for (int i = 0; ((t_cmd_node *)*current_node)->args[i]; i++)
        {
            if (i == 0 && ((t_op_node *)parent)->op_presentation == HEREDOC)
            {
                continue;
            }
            CATCH_ONFAILURE(
                expand(db, &((t_cmd_node *)*current_node)->args[i], tracker->quotes)
            , FAILURE);
        }

    }
    gc_free(db, tracker);
    return SUCCESS; 
}
