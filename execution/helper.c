#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

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
    (void)db;
    DIR *dir;

    bool    is_dir;
    if (ft_strcmp(arg, ".") == 0 || ft_strcmp(arg, "..") == 0)
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

bool    node_in_pipe(void    *node)
{
    return (((t_op_node *)CMD->origin) && ((t_op_node *)CMD->origin)->op_presentation == PIPE);
}

bool    node_in_scope(void  *node)
{
    return (CMD->type == CMD_NODE && CMD->is_scope) || (OP->type == OP_NODE && OP->is_scope);
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
