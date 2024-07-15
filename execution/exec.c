#include "main.h"
#include "parsing.h"
#include "exec.h"

/**
 * @brief This function will apply the command
 * @return signal of eather command was applied or not --> this is gonna be helpful in case of && and || operators
 */
int apply(t_db *db, void **current_node)
{
    // int pid;
    (void)db;

    if (!current_node)
        return FAILURE;
    CURR_CMD->cmd_path = CURR_CMD->args[0];
    if (is_relative_path(CURR_CMD->cmd_path))
    {
        if (access(CURR_CMD->cmd_path, F_OK) != 0)
        {
            perror(CURR_CMD->cmd_path);
        }
    }
    // else if (!is_absolute_path(CURR_CMD->cmd_path))
    // {
    //     if (access(CURR_CMD->cmd_path, F_OK) != 0)
    //     {
    //         CURR_CMD->cmd_path = cmd_path(db, CURR_CMD->cmd_path);
    //         if (!CURR_CMD->cmd_path)
    //             return error(db, "Command not found");
    //     }
    // }
    return SUCCESS;
}

/**
 * @brief This function will execute the each command in the tree
 * @return signal SUCCESS or FAILURE
 */
int exec(t_db *db, void **current_node)
{
    SIGNAL path_signal;

    if (!current_node)
        return FAILURE;

    if (CURR_OP->type == CMD_NODE)
    {
        return apply(db, current_node);
    }
    else if (CURR_OP->type == OP_NODE)
    {
        for (int i = 0; i < CURR_OP->n_childs; i++)
        {
            path_signal = exec(db, &CURR_OP->childs[i]);
            if (path_signal == FAILURE && CURR_OP->op_presentation == AND)
            {
                return FAILURE;
            }
        }
    }
    return SUCCESS;
}