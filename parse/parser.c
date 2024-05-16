#include "../includes/main.h"

void    generate_cmds(char **cmds, t_root *root);
// void    print_root(t_root *root);

void    parse(char *line, t_root *root)
{
    char    **cmds;

    root = malloc(sizeof(t_root));
    if (!root)
        exit(1);
    root->num_commands = -1;
    root->commands = NULL;

    cmds = ft_split(line, '|');
    if (!cmds)
        error("malloc failed", root);
    /*for each command lines here*/
    while (cmds[++root->num_commands]);
    root->commands = malloc(sizeof(t_command *) * root->num_commands);
    if (!root->commands)
        error("malloc failed", root);
    generate_cmds(cmds, root);
}

int is_white_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

void    validate_cmd(char *cmd, t_root *root)
{
    int     i;
    char    *buffer;
    char    *tmp;
    char    *current;

    buffer = NULL;
    i = 0;
    while (cmd[i])
    {
        while (is_white_space(cmd[i]) && cmd[i])
            i++;
        if (!cmd[i])
            break ;
        if (cmd[i] != '\"' && cmd[i] != '\'')
        {
            current = malloc(sizeof(char) * 2);
            if (!current)
                error("malloc failed", root);
            current[0] = cmd[i];
            current[1] = '\0';
            tmp = ft_strjoin(buffer, current);
            if (!tmp)
                error("malloc failed", root);
            free(buffer);
            free(current);
            buffer = tmp;
        }
        i++;
    }
    root->commands[root->num_commands]->cmd = buffer;
    printf("(off) cmd: %s\n", root->commands[root->num_commands]->cmd);
}

void    generate_cmds(char **cmds, t_root *root)
{
    int     i;

    root->num_commands = 0;
    i = -1;
    while (cmds[++i])
    {
        root->commands[i] = malloc(sizeof(t_command));
        if (!root->commands[i])
            error("malloc failed", root);
        validate_cmd(cmds[i], root);
        root->num_commands ++; 
    }
}
