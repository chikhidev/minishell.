#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

/**
 * @brief This function will execute the each command in the tree
 * @return signal SUCCESS or FAILURE
 */

int not_first_child(int child_idx)
{
    return (child_idx != 0);
}
int not_last_child(int  child_idx, int  n_childs)
{
    return (child_idx != n_childs - 1);
}

int child(t_db *db,    int *read_fd,    t_op_node   *node,  int child_i)
{
    t_cmd_node *command = (t_cmd_node *)OP->childs[child_i];
    char    **args = command->args;
    
    if (child_i != 0)
    {
        dup2(*read_fd, STDIN_FILENO); 
    }
    if (child_i != node->n_childs - 1)
        dup2(db->pipe[1], STDOUT_FILENO);
    close(db->pipe[0]);
    close(db->pipe[1]);
    execve(ft_strjoin(db, "/bin/", args[0]), args, NULL);
    return (SUCCESS);
}
int parent(t_db *db,    int *read_fd,    int n_childs,  int child_i)
{
    if (child_i == 0)
    {
        close(db->pipe[1]);
        *read_fd = db->pipe[0];
        pipe(db->pipe);
    }
    else if (child_i == n_childs - 1)
    {
        close(*read_fd);
        close(db->pipe[0]);
        close(db->pipe[1]);
        *read_fd = db->pipe[0];
    }
    else
    {
        close(*read_fd);
        close(db->pipe[1]);
        *read_fd = db->pipe[0];
        pipe(db->pipe);
    }
    return (SUCCESS);
}

int handle_pipe_op(t_db *db,    void    *node)
{
    int i;
    int read_fd;

    db->pids = malloc((OP->n_childs + 1) * (sizeof(pid_t)));
    db->pids[OP->n_childs] = -2;

    i = 0;
    if (pipe(db->pipe) == -1)
        return (gc_void(db), error(db, NULL, "dup"));
    read_fd = db->pipe[0];
    while (i < OP->n_childs)
    {
        int id = fork();
        if (id == CHILD)
            child(db, &read_fd, OP, i);
        else
            parent(db, &read_fd, OP->n_childs, i);
        i++;
    }
    while (i < OP->n_childs)
    {
        wait(NULL);
        i++;
    }
    close(read_fd);
    return (SUCCESS);
}

int handle_op_node(t_db    *db,    void    *node)
{
    if (OP->op_presentation == AND)
        printf("OP->> AND\n");
    else if (OP->op_presentation == OR)
        printf("OP->> OR\n");
    else if (OP->op_presentation == PIPE)
    {
        if (handle_pipe_op(db, node) == FAILURE)
        {
            dprintf(2, "error\n");
            return (FAILURE);
        }
    }
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
        if (handle_op_node(db, node) == FAILURE)
            return error(db, NULL, "pipe");
    }
    for (int i = 0; i < OP->n_childs; i++)
        exec(db, OP->childs[i]);
    return (SUCCESS);
}
