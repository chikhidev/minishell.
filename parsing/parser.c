#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function will split the line into commands and operators
 * depending on the priority of the operators
 * @return 1 on success, 0 on failure
*/
int smart_split(t_db *db, char *line)
{
    (void)db;
    (void)line;

    printf("operators counted: (&&: %d, ||: %d, |: %d, >: %d, <: %d)\n", db->and_count, db->or_count, db->pipe_count, db->redir_count, db->input_count);
    // int i;

    // i = 0;
    // skip_spaces(line, &i);
    // while (line[i])
    // {
    //     printf("salam\n");
    // }
    return (SUCCESS);
}

void    count_operators(t_db *db, char *line)
{
    int i;

    i = 0;
    while (line[i])
    {
        if (line[i] == '&' && line[i + 1] && line[i + 1] == '&')
            db->and_count++;
        if (line[i] == '|' && line[i + 1] && line[i + 1] == '|')
            db->or_count++;
        if (line[i] == '|')
            db->pipe_count++;
        if (line[i] == '>')
            db->redir_count++;
        if (line[i] == '<')
            db->input_count++;
        i++;
    }
}

/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/
int parser(t_db *db, char *line)
{
    count_operators(db, line);
    if (smart_split(db, line) == FAILURE) return (FAILURE);
    return (SUCCESS);
}
