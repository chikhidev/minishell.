#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

void ft_close(t_db *db, int    *fd)
{
    int res;

    res = 0;
    if (*fd != CLOSED)
        res = close(*fd);
    *fd = CLOSED;
    res = INVALID;
    if (res != INVALID)
        return;
    gc_void(db);
    ec_void(db);
    dprintf(2, "close failed\n");
    exit(1);
}

int get_pipes_count(int **pipes)
{
    int i;

    i = 0;
    if (!pipes || !pipes[0])
        return -1;
    while (pipes[i])
        i++;
    return i;
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

int    **prepare_pipes(t_db  *db,   int n_pipes)
{
    int **pipes;
    int i;

    i = 0;
    pipes = gc_malloc(db, (sizeof(int *)) * (n_pipes + 1));
    while (i < n_pipes)
    {
        pipes[i] = gc_malloc(db, sizeof(int) * 2);
        pipe(pipes[i]);
        i++;
    }
    pipes[n_pipes] = NULL;
    return pipes;
}
void    waiter(t_db *db, int  *status)
{
    t_ip_addrs  *ip;

    ip = db->ip;
    while (ip)
    {
        waitpid(ip->ip_addr, status, 0);
        ip = ip->next;
    }
    ip_void(db);
    catch_feedback(db, *status);
}

int dup_pipes(int **pipes, int   index) // index -> 2
{
    if (index == -1)
        return (SUCCESS);
    int n_pipes = get_pipes_count(pipes);
    if (index == 0)
        dup2(pipes[0][1], STDOUT_FILENO);
    else if (index == n_pipes) // 2 == 2
    {
        dup2(pipes[n_pipes - 1][0], STDIN_FILENO);
    }
    else
    {
        dup2(pipes[index - 1][0], STDIN_FILENO);
        dup2(pipes[index][1], STDOUT_FILENO);
    }
    return (SUCCESS);
}


int close_all_pipes(t_db  *db, int    **pipes)
{
    int pipe_i;
    int n_pipes;

    n_pipes = get_pipes_count(pipes);
    pipe_i = 0;

    if (!pipes || !pipes[0])
        return (SUCCESS);
    while (pipe_i < n_pipes)
    {
        ft_close(db, &pipes[pipe_i][0]);
        ft_close(db, &pipes[pipe_i][1]);
        pipe_i++;
    }
    return (SUCCESS);
}

int handle_pipe_op(t_db *db, void *node)
{
    int i;
    int status;
    int **pipes;
    i = 0;
    status = 0;
    
    pipes = prepare_pipes(db, OP->n_childs - 1);
    while (i < OP->n_childs)
    {
        handle_cmd_node(db, OP->childs[i], pipes, i);
        i++;
    }
    close_all_pipes(db, pipes);
    waiter(db, &status);
    catch_feedback(db, status);
    return (SUCCESS);
}

int dup_cmd_io(t_db *db, t_cmd_node *command)
{
    if (command->input_fd != INVALID && command->input_fd != STDIN_FILENO)
    {
        dup2(command->input_fd, STDIN_FILENO);
        ft_close(db, &command->input_fd);
    }

    if (command->output_fd != INVALID && command->output_fd != STDOUT_FILENO)
    {
        dup2(command->output_fd, STDOUT_FILENO);
        ft_close(db, &command->output_fd);
    }
    return (SUCCESS);
}


int exec_cmd(t_db *db, void *node, int **pipes, int index)
{
    char **env_arr;
    char *path;

    if (CMD->input_fd == INVALID || CMD->output_fd == INVALID)
        return FAILURE;


    path = get_path(db, CMD->args);
    env_arr = env_list_to_env_arr(db);
    
    dup_pipes(pipes, index);
    close_all_pipes(db, pipes);
    dup_cmd_io(db, node);
    execve(path, CMD->args, env_arr);
    exit(127);
}

int handle_builtin(t_db *db, void *node, int **pipes, int index)
{
    int status;
    if (index == -1)
        return run_builtin(db, node, index);
    int id = fork();
    if (id == CHILD)
    {
        dup_pipes(pipes, index);
        dup_cmd_io(db, node);
        close_all_pipes(db, pipes);
        status = run_builtin(db, node, index);
        catch_feedback(db, status);
        exit(db->last_signal);
    }
    else
    {
        ip_add(db, id);
    }
    return db->last_signal;
}

int handle_cmd_node(t_db *db, void *node, int **pipes, int index)
{
    int id;
    int status;

    if (is_built_in(node))
        return handle_builtin(db, node,pipes, index);
    id = fork();
    if (id == CHILD)
    {
        if (exec_cmd(db, node, pipes, index) == FAILURE)
            exit(1);
    }
    else
    {
        if (index == -1)
        {
            waitpid(id, &status, 0);
            catch_feedback(db, status);
        }
        else
            ip_add(db, id);
    }
    return (SUCCESS);
}

int exec(t_db   *db, void *node)
{
    if (!node)
        return (SUCCESS);
    if (CMD->type == CMD_NODE)
        return handle_cmd_node(db, node, NULL, -1);
    else if (OP->op_presentation == PIPE)
        return handle_pipe_op(db, node);
    return (SUCCESS);
}