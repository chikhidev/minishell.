#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function will count the number of operators in the line
 * so we can split the line into commands and operators later
*/
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
 * @details This function will split the line into commands and operators
 * depending on the priority of the operators
 * @return 1 on success, 0 on failure
*/
int smart_split(t_db *db, char *line)
{
    int i;

    (void)db;
    i = 0;
    skip_spaces(line, &i);
    while (line[i])
    {
        printf("line[%d] = %c\n", i, line[i]);
        i++;
    }
    return (SUCCESS);
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
