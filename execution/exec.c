#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

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

int exec_builtin(t_db   *db,t_cmd_node *node)
{
    if (ft_strcmp(CMD->args[0], "echo") == 0)
        return echo(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "export") == 0)
        return export(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "pwd") == 0)
        return pwd(db);
    else if (ft_strcmp(CMD->args[0], "env") == 0)
        return env(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "cd") == 0)
        return cd(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "unset") == 0)
        unset(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "exit") == 0)
    {
        error(db, NULL, NULL);
        ec_void(db);
        exit(0);
    }
    return 1;
}


int exec(t_db   *db, void *node)
{
    if (!node)
        return SUCCESS;
    if (CMD->type == CMD_NODE)
    {
        if (is_built_in(node))
        {
            exec_builtin(db, CMD);
            
        }
        else
        {
            printf("CMD->> ");
            for (int i = 0; CMD->args[i]; i++) {
                printf("[%s] ", CMD->args[i]);
            }
            if (CMD->input_fd != STDIN_FILENO)
                printf("IN: %d ", CMD->input_fd);
            if (CMD->output_fd != STDOUT_FILENO)
                printf("OUT: %d", CMD->output_fd);
            printf("\n");
        }
        return SUCCESS;
    }
    else if (OP->type == OP_NODE)
    {
        printf(MAGENTA);
        if (OP->op_presentation == AND) printf("OP->> AND\n");
        else if (OP->op_presentation == OR) printf("OP->> OR\n");
        else if (OP->op_presentation == PIPE) printf("OP->> PIPE\n");
        printf(RESET);
    }
    
    for (int i = 0; i < OP->n_childs; i++)
    {
        exec(db, OP->childs[i]);
    }
    return SUCCESS;
}