#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

int ft_close(int    *fd)
{
    if (*fd != CLOSED)
        close(*fd);
    *fd = CLOSED;
    return (SUCCESS);
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
int dup_cmd_io(t_cmd_node *command)
{
    if (command->input_fd != INVALID && command->input_fd != STDIN_FILENO)
    {
        dup2(command->input_fd, STDIN_FILENO);
        ft_close(&command->input_fd);
    }

    if (command->output_fd != INVALID && command->output_fd != STDOUT_FILENO)
    {
        dup2(command->output_fd, STDOUT_FILENO);
        ft_close(&command->output_fd);
    }

    return (SUCCESS);
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
    if (!pipes)
        return SUCCESS;
    int n_pipes = get_pipes_count(pipes);
    if (index == 0)
    {
        dprintf(2, "%d is standard output F index %d\n", pipes[0][1], index);
        if (pipes[0][1] != CLOSED)
            dup2(pipes[0][1], STDOUT_FILENO);
    }
    else if (index == n_pipes) // 2 == 2
    {
        dprintf(2, "%d is standard input L index %d\n", pipes[n_pipes - 1][0], index);
        if (pipes[n_pipes - 1][0] != CLOSED)
            dup2(pipes[n_pipes - 1][0], STDIN_FILENO);
    }
    else
    {
        dprintf(2, "%d is standard input B, index %d\n", pipes[index - 1][0], index);
        dprintf(2, "%d is standard output B , index %d\n", pipes[index][1], index);
        if (pipes[index - 1][0] != CLOSED)
            dup2(pipes[index - 1][0], STDIN_FILENO);
        if (pipes[index][1] != CLOSED)
            dup2(pipes[index][1], STDOUT_FILENO);
        
    }
    return (SUCCESS);
}


int close_all_pipes(t_db  *db, int    **pipes)
{
    (void)db;
    int pipe_i;
    int n_pipes;

    if (!pipes || !pipes[0])
        return (SUCCESS);
    n_pipes = get_pipes_count(pipes);
    pipe_i = 0;
    int n;

    n = n_pipes - 1;
    while (pipe_i < n_pipes)
    {
        (ft_close(&pipes[pipe_i][0]), ft_close(&pipes[pipe_i][1]));
        pipe_i++;
    }
    return (SUCCESS);
}

int handle_pipe_op(t_db *db,    void    *node,  int **parent_pipes,int  index)
{
    int i;
    int id;
    int status;
    int **pipes;

    i = 0;
    status = 0;
    pipes = NULL;
    if (OP->is_scope)
    {
        id = fork();
        if (id == CHILD)
        {
            // create pipes for the children
            pipes = prepare_pipes(db, OP->n_childs - 1);
            // close parent pipes we will not use them
            dup_pipes(parent_pipes, index);
            close_all_pipes(db, parent_pipes);

            // reset the ip addresses to avoid any confusion
            ip_void(db);

            while (i < OP->n_childs)
            {
                exec(db, OP->childs[i], pipes, i);
                i++;
            }
            close_all_pipes(db, pipes);
            waiter(db, &status);
            exit(db->last_signal);
        }
        else
        {
            waitpid(id, &status, 0);
        }
    }
    else // no scope
    {
        pipes = prepare_pipes(db, OP->n_childs - 1);
        // close parent pipes we will not use them
        dup_pipes(parent_pipes, index);
        close_all_pipes(db, parent_pipes);

        // reset the ip addresses to avoid any confusion
        ip_void(db);
        while (i < OP->n_childs)
        {
            exec(db, OP->childs[i], pipes, i);
            i++;
        }
        close_all_pipes(db, pipes);
        waiter(db, &status);
    }
    catch_feedback(db, status);
    return (SUCCESS);
}


int exec_cmd(t_db *db, void *node, int **pipes, int index)
{
    char **args;
    char **env_arr;
    char *path;
    int signal_catcher;

    if (CMD->input_fd == INVALID || CMD->output_fd == INVALID)
        return FAILURE;

    signal_catcher = 0;
    path = get_path(db, CMD->args);
    env_arr = env_list_to_env_arr(db);
    
    args = CMD->args;
    if (index != -1)
    {
        dup_pipes(pipes, index);
        close_all_pipes(db, pipes);
    }
    // dup_cmd_io(node);
    execve(path, CMD->args, env_arr);
    gc_void(db);
    ec_void(db);
    exit(127);
}

int handle_cmd_node(t_db *db, void *node, int **pipes, int index)
{
    int id;
    int status;
    t_quote *q;

    q = NULL;
    track_quotes(db, &q, CMD->line);
    CMD->args = tokenize(db, &q, CMD->line);
    if (CMD->args == NULL)
        return SUCCESS;

    id = fork();
    if (id == CHILD)
    {
        if (exec_cmd(db, node, pipes, index) == FAILURE)
        {
            exit(1);
        }
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

int exec(t_db   *db, void *node, int    **pipes,   int index)
{

    if (!node)
        return (SUCCESS);

    if (CMD->type == CMD_NODE)
        return handle_cmd_node(db, node, pipes, index);
    else if (OP->op_presentation == PIPE)
        return handle_pipe_op(db, node, pipes, index);
    return (SUCCESS);
}