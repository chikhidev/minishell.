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

int handle_dup(t_db     *db,    int  child_i,	int	temp_fd0)
{
	dup2(db->pipe[1], STDOUT_FILENO);
    if (child_i == 0)
    {
        // dup infile  dup2(e)
        // dup2()
    }
    else
    {
        dup2(temp_fd0, STDIN_FILENO);
    }
	return (SUCCESS);
}

int handle_pipe_op(t_db *db,    void    *node)
{
    int i;
    int tmp_fd0;
    char    *path;
    char    **args;
    char    **env;

    db->pids = malloc((OP->n_childs + 1) * (sizeof(pid_t)));
    db->pids[OP->n_childs] = -2;
    i = 0;
    if (pipe(db->pipe) == -1)
        return (FAILURE);
    close(db->pipe[1]);
    tmp_fd0 = db->pipe[0];
    while (i < OP->n_childs)
    {
        db->pids[i] = fork();
        if (db->pids[i] == 0)
        {
			handle_dup(db,	i,	tmp_fd0);
            args = ((t_cmd_node*)OP->childs[i])->args;
            printf("args[0]  %s\n", args[0]);
            path = cmd_path(db, args[0]);
            env = env_list_to_env_arr(db);
            execve(path, args, env);
            perror(args[0]);
            exit(1);
        }
        else
            waitpid(db->pids[i], NULL, 0);
        i++;
    }
	close(db->pipe[0]);
	close(db->pipe[1]);
	close(tmp_fd0);
    return (SUCCESS);
}

int handle_op_node(t_db    *db,    void    *node)
{
    if (OP->op_presentation == AND)
        printf("OP->> AND\n");
    else if (OP->op_presentation == OR)
        printf("OP->> OR\n");
    else if (OP->op_presentation == PIPE)
        handle_pipe_op(db, node);

    printf(RESET);
    return (SUCCESS);
}

int handle_cmd_node(t_db    *db,    void    *node)
{
    (void) db;
    printf("CMD->> ");
    for (int i = 0; CMD->args[i]; i++) {
        printf("[%s] ", CMD->args[i]);
    }
    if (CMD->input_fd != STDIN_FILENO)
        printf("IN: %d ", CMD->input_fd);
    if (CMD->output_fd != STDOUT_FILENO)
        printf("OUT: %d", CMD->output_fd);
    printf("\n");
    return (SUCCESS);
}

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
        gc_void(db);
        exit(0);
    }
    return 1;
}

int exec(t_db   *db, void *node)
{
    if (!node)
        return (SUCCESS);
    if (CMD->type == CMD_NODE)
    {
        if (is_built_in(node))
            exec_builtin(db, CMD);
        else
			handle_cmd_node(db, node);
    }
    else if (OP->type == OP_NODE)
    {
        handle_op_node(db, node);
    }
    for (int i = 0; i < OP->n_childs; i++)
        exec(db, OP->childs[i]);
    return (SUCCESS);
}
