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
    if (len > 0)
    {
        splitted[k] = gc_malloc(db, sizeof(char) * (len + 1));
        CATCH_NULL(splitted[k]);
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

    if (ft_strlen(line) == 0) return SUCCESS;
    tracker = gc_malloc(db, sizeof(t_tracker));
    if (!tracker) return error(db, "Failed to allocate memory");
    tracker->paranthesis = NULL;
    tracker->quotes = NULL;
    CATCH_FAILURE(track_quotes(db, &tracker->quotes, line));
    CATCH_FAILURE(track_paranthesis(db, &tracker->paranthesis, line, tracker->quotes));
    op = strongest_operator(line, tracker);
    if (tracker->paranthesis && op == NOT_FOUND)
    {
        smart_split(db, remove_paranthesis(db, line, tracker->paranthesis), current_node, parent);
    }
    else if (op != NOT_FOUND)
    {
        CATCH_ONFAILURE(create_op_node(db, op, current_node, parent), FAILURE);
        ((t_op_node *)*current_node)->childs = gc_malloc(db, sizeof(void *) * 
            count_between_op(db, line, op, tracker));
        ((t_op_node *)*current_node)->n_childs = count_between_op(db, line, op, tracker);
        splitted = split_line(db, line, ((t_op_node *)*current_node), tracker);
        CATCH_MALLOC(splitted);
        i = 0;
        while (i < ((t_op_node *)*current_node)->n_childs)
        {
            ((t_op_node *)*current_node)->childs[i] = NULL;
            smart_split(db, splitted[i], &((t_op_node *)*current_node)->childs[i], *current_node);
            i++;
        }
    }
    else
    {
        CATCH_ONFAILURE(create_cmd_node(db, current_node, parent), FAILURE);
        ((t_cmd_node *)*current_node)->args = ft_new_split(db, tracker->quotes, line);
        CATCH_MALLOC(((t_cmd_node *)*current_node)->args);
    }
    gc_free(db, tracker);
    return SUCCESS; 
}
