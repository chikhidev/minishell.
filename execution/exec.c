#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

/**
 * @brief This function will execute the each command in the tree
 * @return signal SUCCESS or FAILURE
 */

int safe_close(int  *fd)
{
    if (*fd != -2)
    {
        close(*fd);
        *fd = -2;
    }
    return (SUCCESS);
}
extern int dprintf (int __fd, const char *__restrict __fmt, ...); // remove later

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
    int sig;
    command = (t_cmd_node *)OP->childs[child_i];
    char    **args = command->args;
    char    *path;

    if (expanded(db, args) == FAILURE)
        return FAILURE;

    if (child_i == 0)
    {
        dup2(db->pipe[1], STDOUT_FILENO);
        close(db->pipe[0]);
        close(db->pipe[1]);
    }
    else if (child_i == OP->n_childs - 1)
    {
        dup2(*read_fd, STDIN_FILENO);
        close(db->pipe[0]);
        close(db->pipe[1]);
        close(*read_fd);
        close(2);
    }
    else
    {
        dup2(*read_fd, STDIN_FILENO);
        dup2(db->pipe[1], STDOUT_FILENO);
        close(db->pipe[0]);
        close(*read_fd);
        close(db->pipe[1]);
        close(2);
    }

    // dprintf(2, "args[0] %s %zu\n", args[0], ft_strlen(args[0]));
    if (is_built_in(command))
    {
        sig = exec_builtin(db, command);
        exit(sig);
    }
    else
    {
        path = cmd_path(db, args[0]);
        if (path)
            execve(path, args, NULL);
        else
            execve(args[0], args, NULL);
        perror(args[0]); // If execve fails, print error
        exit(1);
        return (FAILURE);
    }
    return (SUCCESS);
}
int parent(t_db *db,    int *read_fd,    int n_childs,  int child_i)
{
    if (child_i == 0)
    {
        close(db->pipe[1]);
        *read_fd = db->pipe[0];
        if (child_i != n_childs - 2)
            pipe(db->pipe);
    }
    else if (child_i == n_childs - 1)
    {
        close(db->pipe[0]);
        close(db->pipe[1]);
        close(*read_fd);
    }
    else
    {
        close(db->pipe[1]);
        close(*read_fd);
        *read_fd = db->pipe[0];
        pipe(db->pipe);
    }
    return (SUCCESS);
}
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
    while (i < OP->n_childs)
    {
        waitpid(db->pids[i], NULL, 0);
        i++;
    }
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
        waitpid(-1, &signal_catcher, 0);
        db->last_signal = feedback(db, signal_catcher)->signal;
    }
    return (SUCCESS);
}

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
