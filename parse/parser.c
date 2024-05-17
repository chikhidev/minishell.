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

void    extract_cmd(char *cmd, t_root *root, t_quote *quotes, int quotes_count)
{
    int     i;
    int     putted;

    i = 0;
    while (is_white_space(cmd[i]))
        i++;
    while (cmd[i])
    {
        // printf("cmd[%d]: %c\n", i, cmd[i]);
        if (is_white_space(cmd[i]) && !quotes)
            break ;
        if (cmd[i] == '\'' || cmd[i] == '\"')
        {
            putted = 0;
            for (int k = 0; k < quotes_count; k++)
            {
                printf("k->%d\n", k);
                if (quotes[k].ascii == cmd[i] && quotes[k].end == -1)
                {
                    quotes[k].end = i;
                    putted = 1;
                }
            }
            if (!putted)
            {
                quotes = ft_realloc(quotes, sizeof(t_quote) * (quotes_count + 1));
                if (!quotes)
                    error("malloc failed", root);
                quotes[quotes_count].ascii = cmd[i];
                quotes[quotes_count].start = i;
                quotes[quotes_count].end = -1;
                quotes_count++;
            }
        }
        else
            root->buffer = concat_str(root, root->buffer, cmd[i]);
        i++;
    }
    for (int k = 0; k < quotes_count; k++)
    {
        if (quotes[k].end == -1)
        {
            while (quotes[k].end == -1)
            {
                root->tmp = readline("quote> ");
                root->buffer = concat_str(root, root->buffer, '\n');
                extract_cmd(root->tmp, root, quotes, quotes_count);
            }
        }
    }
}

void    generate_cmds(char **cmds, t_root *root)
{
    int     i;
    t_quote *quotes;
    int     quotes_count;

    root->buffer = NULL;
    root->tmp = NULL;
    root->num_commands = 0;
    quotes = NULL;
    quotes_count = 0;
    i = -1;
    while (cmds[++i])
    {
        root->commands[i] = malloc(sizeof(t_command));
        if (!root->commands[i])
            error("malloc failed", root);
        extract_cmd(cmds[i], root, quotes, quotes_count);
        root->commands[root->num_commands]->cmd = root->buffer;
        printf("(off) cmd: $%s$\n", root->commands[root->num_commands]->cmd);
        free(quotes);
        root->num_commands ++; 
    }
}
