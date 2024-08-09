#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"


/**
 logic to track if the command path is relative or absolute
    if the path is relative ->
        check the permisssions and just execute it :)
    else when the path is absolute then ->
        check the permissions and just execute it ;)
    else it just a regular command so check if it's there and execute it
*/
int child_dup(t_db *db, int  index,void    *node)
{
    if (index == 0)
    {
        dup2(db->pipe[1], STDOUT_FILENO);
        close(db->pipe[0]);
        close(db->pipe[1]);
    }
    else if (index == ((t_op_node*)(CMD->origin))->n_childs - 1)
    {
        dup2(db->read_fd, STDIN_FILENO);
        close(db->pipe[0]);
        close(db->pipe[1]);
        close(db->read_fd);
    }
    else
    {

        dup2(db->read_fd, STDIN_FILENO);
        dup2(db->pipe[1], STDOUT_FILENO);
        close(db->pipe[0]);
        close(db->read_fd);
        close(db->pipe[1]);
    }
    return (SUCCESS);
}

int parnt_dup(t_db *db, int  index,void    *node)
{
    if (index == 0)
    {
        close(db->pipe[1]);
        db->read_fd = db->pipe[0];
        if (index != OP->n_childs - 2)
            pipe(db->pipe);
    }
    else if (index == OP->n_childs - 1)
    {
        close(db->pipe[0]);
        close(db->pipe[1]);
        close(db->read_fd);
    }
    else
    {
        close(db->pipe[1]);
        close(db->read_fd);
        db->read_fd = db->pipe[0];
        pipe(db->pipe);
    }
    return (SUCCESS);
}

char    *get_path(t_db  *db, char    **args)
{
    char    *path;

    path = args[0];
    if (is_relative_path(path) || is_absolute_path(path))
    {
        if (access(path, F_OK) + access(path, X_OK) != 0)
        {
            perror(path);
            exit(error(db, NULL, NULL) + 127 - (access(path, X_OK) != 0));
        }
    }
    else
    {
        path = cmd_path(db, args[0]);
        if (db->error)
            exit(126);
        if (!path)
        {
            error(db, args[0], "command not found ya hamid\n");
            exit(127);
        }
    }
    return (path);
}

int handle_pipe_op(t_db *db,    void    *node)
{
    int i;
    t_ip_addrs  *ip;
    db->pipe[0] = -2;
    db->pipe[1] = -2;
    db->pids = gc_malloc(db, (OP->n_childs) * (sizeof(pid_t)));
    i = 0;
    db->read_fd = -2;
    pipe(db->pipe);
    while (i < OP->n_childs)
    {
        if (exec(db, OP->childs[i], i) == FAILURE)
            return (FAILURE);
        i++;
    }

    i = 0;
    ip = db->ip;
    while (ip)
    {
        waitpid(ip->ip_addr, &db->last_signal, 0);
        ip = ip->next;
    }
    ip_void(db);
    return (SUCCESS);
}
int handle_and_op(t_db *db,    void    *node)
{
    (void) db;
    (void) node;
    return (SUCCESS);
}

int handle_cmd_node(t_db    *db,    void    *node,  int index)
{

    int id;
    t_cmd_node *command;
    char    **args;
    char    **env_arr;
    char    *path;
    int signal_catcher;

    command = (t_cmd_node  *)node;
    signal_catcher = 0;
    id = fork();
    if (id == CHILD)
    {
        if (((t_op_node *)CMD->origin) && ((t_op_node *)CMD->origin)->op_presentation == PIPE)
            child_dup(db, index, node);
        args = command->args;
        env_arr = env_list_to_env_arr(db);
        path = get_path(db, args);
        execve(path, args, env_arr);
        exit(127);
    }
    else
    {
        if ( ((t_op_node *)CMD->origin) && ((t_op_node *)CMD->origin)->op_presentation == PIPE)
            parnt_dup(db, index, node);
        if ((t_op_node *)CMD->origin == NULL) // no parent means wait directly
            waitpid(id, &db->last_signal, 0);
        else
            ip_add(db, id); // else add it to linked list to wait it later
        db->last_signal = feedback(db, signal_catcher)->signal;
    }
    return (SUCCESS);
}

/**
 * @brief This function will execute a built in
 * @return signal SUCCESS or FAILURE
 */

int exec_builtin(t_db   *db,t_cmd_node *node)
{
    if (ft_strcmp(CMD->args[0], "echo") == 0)
        return echo_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "export") == 0)
        return export_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "pwd") == 0)
        return pwd_(db);
    else if (ft_strcmp(CMD->args[0], "env") == 0)
        return env_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "cd") == 0)
        return cd_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "unset") == 0)
        return unset_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "exit") == 0)
        exit_(db,   CMD->args);
    return FAILURE;
}


/**
 * @brief This function will execute the each command in the tree
 * @return signal SUCCESS or FAILURE
 */

int exec(t_db   *db, void *node,    int index)
{
    if (!node)
        return (SUCCESS);
    if (CMD->type == CMD_NODE)
    {
        if (expanded(db, CMD->args) == FAILURE)
            return FAILURE;
        if (is_built_in(node))
            return exec_builtin(db, CMD);
        else
			return handle_cmd_node(db, node, index);
    }
    else if (OP->op_presentation == PIPE)
    {
        if (handle_pipe_op(db, node) == FAILURE)
            return (FAILURE);
        if (handle_and_op(db, node) == FAILURE)
            return (FAILURE);
    }

    return (SUCCESS);
}
