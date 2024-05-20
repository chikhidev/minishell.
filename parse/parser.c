#include "../includes/main.h"

void    extract_cmd(char *cmd, t_root *root, int quotes_count)
{
    int     i;

    i = 0;
    while (is_white_space(cmd[i]))
        i++;
    while (cmd[i])
    {
        if (root->error)
            return ;
        if (is_white_space(cmd[i]) && !root->quotes)
            break ;
        quotes_tracking(root, cmd[i], i, &quotes_count);
        root->tmp = concat_str(root, root->buffer, cmd[i]);
        if (!root->tmp || root->error)
            return ;
        free(root->buffer);
        root->buffer = root->tmp;
        i++;
    }
    quotes_checker(root, quotes_count, extract_cmd);
}

void    generate_cmds(t_root *root)
{
    int     i;
    int     quotes_count;

    root->buffer = NULL;
    root->tmp = NULL;
    root->quotes = NULL;
    root->num_commands = 0;
    quotes_count = 0;
    i = -1;
    while (!root->error && root->cmds && root->cmds[++i])
    {
        root->commands[i] = malloc(sizeof(t_command));
        if (!root->commands[i])
            return (error("malloc failed", root));
        root->commands[i]->args = NULL;
        extract_cmd(root->cmds[i], root, quotes_count);
        if (root->error)
            return ;
        root->commands[root->num_commands]->cmd = root->buffer;
        root->buffer = NULL;
        root->tmp = NULL;
        printf("(off) cmd: [%s]\n", root->commands[root->num_commands]->cmd);
        free(root->quotes);
        root->quotes = NULL;
        root->num_commands++;
    }
}

void    parse(char *line, t_root *root)
{
    root->cmds = ft_split(line, '|');
    if (!root->cmds)
        return error("malloc failed", root);
    while (root->cmds[++root->num_commands]);
    root->commands = malloc(sizeof(t_command *) * root->num_commands);
    if (!root->commands)
        return error("malloc failed", root);
    generate_cmds(root);
}
