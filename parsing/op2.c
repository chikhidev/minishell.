#include "../includes/main.h"
#include "../includes/parsing.h"

int is_op(char *line, int *i)
{
    if (line[*i] == '&' && line[*i + 1] && line[++(*i)] == '&')
        return AND;
    else if (line[*i] == '|' && line[*i + 1] && line[++(*i)] == '|')
        return OR;
    else if (line[*i] == '>' && line[*i + 1] && line[++(*i) == '>'])
        return APPEND;
    else if (line[*i] == '<' && line[*i + 1] && line[++(*i) == '<'])
        return HEREDOC;
    else if (line[*i] == '|')
        return PIPE;
    else if (line[*i] == '>')
        return REDIR;
    return INVALID;
}

int priority_of_op(int op)
{
    if (op == AND || op == OR) return AND_OR_PRIORITY;
    else if (op == PIPE) return PIPE_PRIORITY;
    else if (op == REDIR || op == APPEND) return REDIR_PRIORITY;
    else if (op == HEREDOC) return REDIR_PRIORITY;
    return NOT_FOUND;
}