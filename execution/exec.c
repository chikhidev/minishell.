#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"


bool    node_in_scope(void  *node)
{
    return (CMD->type == CMD_NODE && CMD->is_scope) || (OP->type == OP_NODE && OP->is_scope);
}

// int run_cmd(t_db    *db, int index,  void    *node)
// {
//     int id;
//     t_cmd_node *command;
//     char    **args;
//     char    **env_arr;
//     char    *path;
//     int signal_catcher;

//     command = (t_cmd_node  *)node;
//     signal_catcher = 0;
//     if (is_built_in(node))
//         exec_builtin(db, node, index);
//     else
//     {
//         id = fork();
//         if (id == CHILD)
//         {
//             if (node_in_pipe(node))
//                 child_dup(db, index, node);
//             args = command->args;
//             env_arr = env_list_to_env_arr(db);
//             path = get_path(db, args);
//             handle_redirections(db, node);
//             if (CMD->input_fd != -1 && CMD->output_fd != -1)
//                 execve(path, args, env_arr);
//             else
//                 exit(1);
//             if (!handle_is_dir(db, path))
//                 perror(args[0]);
//             exit(127);
//         }
//         else
//         {
//             if (node_in_pipe(node))
//                 parnt_dup(db, index, node);
//             if ((t_op_node *)CMD->origin == NULL)
//                 waitpid(id, &signal_catcher, 0);
//             else
//                 ip_add(db, id);
//             db->last_signal = feedback(db, signal_catcher)->signal;
//         }
//     }
//     return (SUCCESS);
// }

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
    t_op_node   *pipe_node;

    pipe_node = CMD->origin;
    if (index == 0)
    {
        dup2(db->pipe[1], STDOUT_FILENO);
        close(db->pipe[0]);
        close(db->pipe[1]);
    }
    else if (index == pipe_node->n_childs - 1)
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
    t_op_node   *pipe_node;

    pipe_node = CMD->origin;
    if (index == 0)
    {
        close(db->pipe[1]);
        db->read_fd = db->pipe[0];
        if (index != pipe_node->n_childs - 2)
            pipe(db->pipe);
    }
    else if (index == pipe_node->n_childs - 1)
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
    if (ft_strcmp(path, ".") == 0 || ft_strcmp(path, "..") == 0 || is_str_empty(db, path))
        return ft_strdup(db, path);
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
            error(db, args[0], "command not found ya hamid");
            exit(127);
        }
    }
    return (path);
}

bool    node_in_pipe(void    *node)
{
    return (((t_op_node *)CMD->origin) && ((t_op_node *)CMD->origin)->op_presentation == PIPE);
}

int handle_pipe_op(t_db *db,    void    *node)
{
    int i;
    int status;
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
        waitpid(ip->ip_addr, &status, 0);
        ip = ip->next;
    }
    ip_void(db);
    db->last_signal = feedback(db, status)->status;
    return (SUCCESS);
}

int handle_and_op(t_db *db,    void    *node)
{
    int i;
    int status;
    bool op_in_scope;
    i = 0;
    if (OP->is_scope)
        op_in_scope = true;
    while (i < OP->n_childs)
        {
            if (exec(db, OP->childs[i], i) == FAILURE)
                return (FAILURE);
            if (((t_cmd_node *) OP->childs[i])->type == CMD_NODE)
            {
                wait(&status);
                db->last_signal = feedback(db, status)->status;
            }
            if (db->last_signal != 0)
                return (FAILURE);
            i++;
        }
    return (SUCCESS);
}

int handle_or_op(t_db *db,    void    *node)
{
    int i;
    int status;
    i = 0;
    while (i < OP->n_childs)
    {
        if (exec(db, OP->childs[i], i) == FAILURE)
            return FAILURE;
        if (((t_cmd_node *) OP->childs[i])->type == CMD_NODE)
        {
            wait(&status);
            db->last_signal = feedback(db, status)->status;
        }
        if (db->last_signal == 0)
            return (FAILURE);
        i++;
    }
    return (SUCCESS);
}

int handle_redirections(t_db    *db,    void    *node)
{
    (void) db;
    if (CMD->input_fd != 0)
    {
        dup2(CMD->input_fd, STDIN_FILENO);
        close(CMD->input_fd);
    }
    if (CMD->output_fd != 1)
    {
        dup2(CMD->output_fd, STDOUT_FILENO);
        close(CMD->output_fd);
    }
    return (SUCCESS);
}

int handle_is_dir(t_db *db, char    *arg)
{
    DIR *dir;

    bool    is_dir;
    if (is_str_empty(db, arg) || ft_strcmp(arg, ".") == 0 || ft_strcmp(arg, "..") == 0)
        return (printf("%s: command not found\n", arg), 1);
    is_dir = false;

    dir = opendir(arg);
    if (dir)
    {
        printf("%s Is a directory\n", arg);
        is_dir = 1;
    }
    if (closedir(dir) == -1)
        return -1;
    return is_dir;
}

void    child_slash_sig_handler(int sig)
{
    (void)sig;
    printf("Caught signa in child l\n");
}

/*
    if in built in then execute it
        (it forks inside the built in if node inside of a pipe)
    else (cmd): execute it +
        1- no parent means wait directly
        2- else add it to linked list to wait it later

*/



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
    if (is_built_in(node))
        exec_builtin(db, node, index);
    else
    {
        id = fork();
        if (id == CHILD)
        {
            if (node_in_pipe(node))
                child_dup(db, index, node);
            args = command->args;
            env_arr = env_list_to_env_arr(db);
            path = get_path(db, args);
            handle_redirections(db, node);
            if (CMD->input_fd != -1 && CMD->output_fd != -1)
                execve(path, args, env_arr);
            else
                exit(1);
            if (!handle_is_dir(db, path))
                perror(args[0]);
            exit(127);
        }
        else
        {
            if (node_in_pipe(node))
                parnt_dup(db, index, node);
            if ((t_op_node *)CMD->origin == NULL)
                waitpid(id, &signal_catcher, 0);
            else
                ip_add(db, id); //
            db->last_signal = feedback(db, signal_catcher)->signal;
        }
    }
    return (SUCCESS);
}


int run_builtin(t_db   *db,t_cmd_node *node)
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
    return 127;
}

//     if (CMD->input_fd != 0)
//         dup2(CMD->input_fd, STDIN_FILENO);
//     if (CMD->output_fd != 1)
//         dup2(CMD->output_fd, STDOUT_FILENO);


int exec_builtin(t_db   *db,t_cmd_node *node, int   index)
{
    int id;
    int signal_catcher;
    signal_catcher = 0;

    if (node_in_pipe(node)) // if inside a pipe then we need to fork
    {
        id = fork();
        if (id == CHILD) // if child (dup and execute it and get signal)
        {
            child_dup(db, index, node);
            handle_redirections(db, node);
            signal_catcher = run_builtin(db, node);
            exit(signal_catcher);
        }
        else // in parent we dup back and wait
        {
            parnt_dup(db, index, node);
            if ((t_op_node *)CMD->origin == NULL) // no parent means wait directly
                waitpid(id, &signal_catcher, 0);
            else
                ip_add(db, id); // else add it to linked list to wait it later
            db->last_signal = feedback(db, signal_catcher)->signal;
        }
    }
    else // not inside a pipe, only run it, with no forking
    {

        db->stdout_dup = dup(STDOUT_FILENO);
        db->stdin_dup = dup(STDIN_FILENO);
        handle_redirections(db, node);
        db->last_signal = run_builtin(db, node);
        dup2(db->stdin_dup, STDIN_FILENO);
        dup2(db->stdout_dup, STDOUT_FILENO);
        close(db->stdin_dup);
        close(db->stdout_dup);
    }
    return SUCCESS;
}


/**
 * @brief This function will execute the each command in the tree
 * @return signal SUCCESS or FAILURE
 */

int handle_node(t_db   *db, void *node,    int index)
{
    if (CMD->type == CMD_NODE)
    {
        if (expanded(db, CMD->args) == FAILURE)
            return FAILURE;
        else
            return handle_cmd_node(db, node, index);
    }
    else if (OP->op_presentation == PIPE)
    {
        if (handle_pipe_op(db, node) == FAILURE)
            return (FAILURE);
    }
    else if (OP->op_presentation == AND)
    {
        if (handle_and_op(db, node) == FAILURE)
            return (FAILURE);
    }
    else if (OP->op_presentation == OR)
    {
        if (handle_or_op(db, node) == FAILURE)
            return (FAILURE);
    }
    return (SUCCESS);
}

int exec(t_db   *db, void *node,    int index)
{
    int id;

    if (!node)
        return (SUCCESS);
    if (node_in_scope(node))
    {
        id = fork();
        if (id == CHILD)
        {
            handle_node(db, node, index);
            exit(db->last_signal);
        }
        else
            wait(NULL);
    }
    else
        handle_node(db, node, index);
    return (SUCCESS);
}


