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

// void    print_root(t_root *root)
// {
//     int i;
//     int j;

//     i = -1;
//     while (++i < root->num_commands)
//     {
//         j = -1;
//         printf("cmd: %s\n", root->commands[i]->cmd);
//         while (root->commands[i]->args[++j])
//             printf("args[%d]: %s\n", j, root->commands[i]->args[j]);
//     }
// }

int is_white_space(char c)
{
    return (c == ' ' || (c >= 9 && c <= 13));
}

void    validate_cmd(char *cmd, t_root *root)
{
    int     i;
    char    *buffer;

    buffer = NULL;
    i = -1;
    while (cmd[++i])
    {
        if (!is_white_space(cmd[i]) && cmd[i] != '|'
            && cmd[i] != '>' && cmd[i] != '\"' && cmd[i] != '\'')
        {
            buffer = ft_realloc(buffer, ft_strlen(buffer) + 1);
            if (!buffer)
                error("malloc failed", root);
            buffer[ft_strlen(buffer) -1] = cmd[i];
            buffer[ft_strlen(buffer)] = '\0';
        }
    }
    root->commands[root->num_commands]->cmd = buffer;
    printf("cmd: %s\n", root->commands[root->num_commands]->cmd);
}

void    generate_cmds(char **cmds, t_root *root)
{
    int     i;

    i = -1;
    while (cmds[++i])
    {
        root->commands[i] = malloc(sizeof(t_command));
        if (!root->commands[i])
            error("malloc failed", root);

        validate_cmd(cmds[i], root);

        exit(0);/*remove this line later*/
    }
}
