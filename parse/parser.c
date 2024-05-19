#include "../includes/main.h"

void    extract_cmd(char *cmd, t_root *root, int quotes_count)
{
    int     i;

    if (root->error)
        return ;
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
        root->buffer = concat_str(root, root->buffer, cmd[i]);
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
            error("malloc failed", root);
        extract_cmd(root->cmds[i], root, quotes_count);
        root->commands[root->num_commands]->cmd = root->buffer;
        printf("(off) cmd: $%s$\n", root->commands[root->num_commands]->cmd);
        free(root->quotes);
        root->quotes = NULL;
        root->num_commands++; 
    }
}

void    parse(char *line, t_root *root)
{
    root = malloc(sizeof(t_root));
    if (!root)
        exit(1);
    root->num_commands = -1;
    root->commands = NULL;
    root->error = 0;

    root->cmds = ft_split(line, '|');
    if (!root->cmds)
        error("malloc failed", root);
    /*for each command lines here*/
    while (root->cmds[++root->num_commands]);
    root->commands = malloc(sizeof(t_command *) * root->num_commands);
    if (!root->commands)
        error("malloc failed", root);
    generate_cmds(root);
}
