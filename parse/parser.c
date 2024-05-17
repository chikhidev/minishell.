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

char *concat_str(t_root *root, char *buffer, char _char_)
{
    char    *tmp;
    char    *current;

    current = malloc(sizeof(char) * 2);
    if (!current)
        error("malloc failed", root);
    current[0] = _char_;
    current[1] = '\0';
    tmp = ft_strjoin(buffer, current);
    if (!tmp)
        error("malloc failed", root);
    free(current);
    free(buffer);
    return tmp;
}

void    extract_cmd(char *cmd, t_root *root)
{
    int     i;
    int     j;
    char    *buffer;
    int     quoted;
    char    quote;
    char    *tmp;

    buffer = NULL;
    i = 0;
    quoted = 0;
    while (is_white_space(cmd[i]))
        i++;
    while (cmd[i])
    {
        if (cmd[i] == '\'' || cmd[i] == '\"')
        {
            quoted = !quoted;
            quote = cmd[i];
        }
        if (is_white_space(cmd[i]) && !quoted)
            break ;
        buffer = concat_str(root, buffer, cmd[i]);
        i++;
    }
    while (quoted)
    {
        printf("\n");
        j = 0;
        tmp = readline("dqoute> ");
        while (tmp[j])
        {
            if ((tmp[j] == '\"' || tmp[j] == '\'') && tmp[j] == quote)
                quoted = 0;
            buffer = concat_str(root, buffer, tmp[j]);
            j++;
        }
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
        extract_cmd(cmds[i], root);
        root->num_commands ++; 
    }
}
