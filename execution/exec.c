#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

/**
 * @brief This function will execute the each command in the tree
 * @return signal SUCCESS or FAILURE
 */

extern int dprintf (int __fd, const char *__restrict __fmt, ...);

int not_first_child(int child_idx)
{
    return (child_idx != 0);
}
int not_last_child(int  child_idx, int  n_childs)
{
    return (child_idx != n_childs - 1);
}

int handle_single_cmd(t_db *db,    void    *node)
{
    int id;
    t_cmd_node *command = (t_cmd_node *)OP->childs[0];
    char    **args = command->args;
    char    *path;

    id = fork();
    if (id == 0)
    {
        path = cmd_path(db, args[0]);
        if (path)
            execve(path, args, NULL);
        else
            execve(args[0], args, NULL);
        return (FAILURE);
    }
    else
        wait(NULL);
    return (SUCCESS);
}


int child(t_db *db,    int *read_fd,    t_op_node   *node,  int child_i)
{
    t_cmd_node *command;

    command = (t_cmd_node *)OP->childs[child_i];
    char    **args = command->args;
    char    *path;
    if (node->n_childs == 1)
        return (handle_single_cmd(db, node));
    
    if (not_last_child(child_i, OP->n_childs))
    {
        dup2(db->pipe[1], STDOUT_FILENO); // now every child except last one will write in the pipe
    }

    if (not_first_child(child_i))
    {
        // dprintf(2,"readfd will be closed and has %d\n", *read_fd);
        dup2(*read_fd, STDIN_FILENO);
        close(*read_fd);
    }
    
    // dprintf(2, "args[0] %s %zu\n", args[0], ft_strlen(args[0]));
    if (is_built_in(command))
        exec_builtin(db, command);
    else
    {
        path = cmd_path(db, args[0]);
        if (path)
            execve(path, args, NULL);
        else
            execve(args[0], args, NULL);
        perror(args[0]); // If execve fails, print error
        return (FAILURE);
    }
    return (SUCCESS);
}
int parent(t_db *db,    int *read_fd,    int n_childs,  int child_i)
{
    if (not_last_child(child_i, n_childs))
    {
        close(*read_fd); // pipe[0];
        *read_fd = db->pipe[0];
        close(db->pipe[1]);
        pipe(db->pipe);
    }
    return (SUCCESS);
}

int handle_pipe_op(t_db *db,    void    *node)
{
    int i;
    int read_fd;
    t_cmd_node *command;

    db->pids = malloc((OP->n_childs + 1) * (sizeof(pid_t)));
    db->pids[OP->n_childs] = -2;

    i = 0;
    if (pipe(db->pipe) == -1)
        return (gc_void(db), error(db, NULL, "dup"));
    while (i < OP->n_childs)
    {
        command = (t_cmd_node *)OP->childs[i];
        // printf("gona fork for %s\n", command->args[0]);
        int id = fork();
        if (id == CHILD)
            child(db, &read_fd, OP, i);
        else
            parent(db, &read_fd, OP->n_childs, i);
        i++;
    }
    i = 0;
    while (i < OP->n_childs)
    {
        wait(NULL);
        i++;
    }
    close(db->pipe[0]);
    close(db->pipe[1]);
    return (SUCCESS);
}

int handle_op_node(t_db    *db,    void    *node)
{
    // printf("OP->> PIPE\n");
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
    int id;
    t_cmd_node *command;
    char    **args;
    char    **env_arr;
    char    *path;

    command = (t_cmd_node  *)node;
    if (command->type != CMD_NODE)
        return (SUCCESS);
     args = command->args;
    id = fork();
    if (id == 0)
    {
        env_arr = env_list_to_env_arr(db);
        path = cmd_path(db, args[0]);
        if (path)
            execve(path, args, env_arr);
        else
            execve(args[0], args, env_arr);
        perror(args[0]);
        return (FAILURE);
    }
    else
        wait(NULL);
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
        return unset(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "exit") == 0)
        exit_(db,   CMD->args);
    return FAILURE;
}

int exec(t_db   *db, void *node)
{
    if (!node)
        return (SUCCESS);
    if (CMD->type == CMD_NODE)
    {
        if (is_built_in(node))
            return exec_builtin(db, CMD);
        else
			return handle_cmd_node(db, node);
    }
    else if (OP->type == OP_NODE)
    {
        if (handle_op_node(db, node) == FAILURE)
            return error(db, NULL, "pipe");
    }

    return (SUCCESS);
}
