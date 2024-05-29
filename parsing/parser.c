#include "../includes/main.h"
#include "../includes/parsing.h"

/**
 * @details This function's job is to find the biggest operator in the line
 * - if there are paranthesis, it should ignore the operators inside the paranthesis
 * @return the biggest operator
*/
// void    *biggest_op(t_db *db, char *line, void *origin)
// {
//     int i;
//     t_op_node   *op;

//     i = 0;
//     op = gc_malloc(db, sizeof(t_op_node));
//     if (!op) return (NULL);
//     op->type = OP_NODE;
//     op->origin = origin; // change later -----------------------
//     op->priority = -1;
//     op->op_presentation = -1;
//     op->childs = NULL;
//     op->n_childs = 0;

//     // find the biggest operator by finding the last one in case of (&& ||)
//     // if there are no && or ||, prioritize the pipe
//     while (line[i])
//     {
//         if (line[i] == '&' && line[i + 1] && line[i + 1] == '&'
//             && op->priority <= AND_OR_PRIORITY && !is_inside_paranthesis(db, i))
//         {
//             op->op_presentation = AND;
//             op->priority = AND_OR_PRIORITY;
//             i++;
//         }
//         else if (line[i] == '|' && line[i + 1] && line[i + 1] == '|'
//             && op->priority <= AND_OR_PRIORITY && !is_inside_paranthesis(db, i))
//         {
//             op->op_presentation = OR;
//             op->priority = AND_OR_PRIORITY;
//             i++;
//         }
//         else if (line[i] == '|' && op->priority <= PIPE_PRIORITY && !is_inside_paranthesis(db, i))
//         {
//             op->op_presentation = PIPE;
//             op->priority = PIPE_PRIORITY;
//         }
//         else if (line[i] == '>' && line[i + 1] && line[i + 1] == '>'
//             && op->priority <= REDIR_PRIORITY && !is_inside_paranthesis(db, i))
//         {
//             op->op_presentation = APPEND;
//             op->priority = REDIR_PRIORITY;
//         }
//         else if (line[i] == '>' && op->priority <= REDIR_PRIORITY && !is_inside_paranthesis(db, i))
//         {
//             op->op_presentation = REDIR;
//             op->priority = REDIR_PRIORITY;
//         }
//         else if (line[i] == '<' && op->priority <= REDIR_PRIORITY && !is_inside_paranthesis(db, i))
//         {
//             op->op_presentation = INPUT;
//             op->priority = REDIR_PRIORITY;
//         }
//         i++;
//     }
//     return (op);
// }

/**
 * @details This function should:
 * 1 - split scopes with paranthesis if available
 * 2 - split using whitespaces and quotes
 * @return 1 on success, 0 on failure
*/
// int smart_split(t_db *db, char *line, void *origin)
// {
//     t_op_node   *main_op;
//     // t_cmd_node  *tmp_cmd;
//     int         i;

//     (void)db;
//     main_op = NULL;
//     i = 0;
//     skip_spaces(line, &i);
//     //decide the main deviser here
//     main_op = biggest_op(db, line, origin);

//     if (!main_op) return (FAILURE);

//     // // DEBUG --------------------------------------------------------
//     printf(RED"\n[DEBUG] main operator: %d\n"RESET, main_op->op_presentation);
//     // // DEBUG --------------------------------------------------------

//     if (main_op->op_presentation != -1)
//     {
        
//         origin = main_op;
        
//     }
//     // else if (main_op->op_presentation == -1)
//     // {
//     //     gc_free(db, main_op);
//     //     tmp_cmd = gc_malloc(db, sizeof(t_cmd_node));
//     //     if (!tmp_cmd) return (FAILURE);
//     //     tmp_cmd->type = CMD_NODE;
//     //     tmp_cmd->origin = NULL;
//     //     tmp_cmd->cmd_path = NULL;
//     //     tmp_cmd->args = whitespaces_split(db, line, i);
//     //     if (!tmp_cmd->args) return (FAILURE);
//     //     origin = tmp_cmd;
//     // }
//     // else
//     //     return error(db, "syntax error near unexpected token `)'");
//     return (SUCCESS);
// }

/**
 * @details This function will parse the line and split it into commands and operators
 * @return 1 on success, 0 on failure
*/
int parser(t_db *db, char *line)
{
    int i;

    if (ft_strlen(line) == 0) return (SUCCESS);
    i = 0;
    skip_spaces(line, &i);
    if (line[i] == '\0') return (SUCCESS);
    if (track_quotes(db, line) == FAILURE) return (FAILURE);
    if (expand(db, &line) == FAILURE) return (FAILURE);
    if (track_paranthesis(db, line) == FAILURE) return (FAILURE);
    if (track_operators(db, line) == FAILURE) return (FAILURE);

    // // DEBUG --------------------------------------------------------
    printf(MAGENTA"\n[DEBUG] line: %s\n"RESET, line);

    // for (t_parnth *p = db->paranthesis; p; p = p->next)
    // {
    //     printf("close -> [%d]    open -> [%d]\n", p->close_, p->open_);
    //     printf("\n");
    // }

    // // DEBUG --------------------------------------------------------

    // !test
    // *test
    // ?test

    // if (unused_ops(line)) return error(db, "syntax error near unexpected token operator");
    // if (!all_scopes_has_op_between(db, line)) return (FAILURE);
    // if (smart_split(db, line, db->root_node) == FAILURE) return (FAILURE);

    return (SUCCESS);
}
