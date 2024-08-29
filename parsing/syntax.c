#include "main.h"
#include "parsing.h"


// int is_op_redir(char *line, int i)
// {
//     if (ft_strncmp(&line[i], ">>", 2) == 0
//         || ft_strncmp(&line[i], "<<", 2) == 0)
//     {
//         printf("Syntax error near unexpected token `%c%c'\n", line[i], line[i + 1]);
//         return (FAILURE);
//     }

//     if (ft_strncmp(&line[i], ">", 1) == 0
//         || ft_strncmp(&line[i], "<", 1) == 0)
//     {
//         printf("Syntax error near unexpected token `%c'\n", line[i]);
//         return (FAILURE);
//     }

//     return (SUCCESS);
// }

bool is_redir(int op)
{
    return (op == APPEND || op == INPUT || op == HEREDOC || op == REDIR);
}

int syntax_checker(t_db *db, char *line, t_quote *quotes)
{
    bool already_found;
    int last;
    int curr;
    int i;

    i = 0;
    last = INVALID;
    curr = INVALID;
    already_found = false;

    skip_open_spaces(quotes, line, &i);
    if (!line[i])
        return (SUCCESS);

    while (line[i])
    {
        curr = is_special(db, line, &i, quotes);
        if (curr != INVALID && curr != W_SPACE)
        {
            if (db->heredoc_counter > 16)
                return error(db, "heredoc", "maximum here-document count exceeded");

            if ((!already_found && curr == PIPE)
                || (last == PIPE && !is_redir(curr))
                || (is_redir(curr) && is_redir(last)))
            {
                printf("Syntax error bad token\n");
                return (FAILURE);
            }
            last = curr;
            already_found = true;
        }
                
        i++;
    }

    if (last != INVALID && last != W_SPACE)
    {
        printf("Syntax error bad token\n");
        return (FAILURE);
    }

    return (SUCCESS);
}
