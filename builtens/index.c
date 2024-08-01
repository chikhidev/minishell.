#include "builtens.h"
#include "parsing.h"

bool is_built_in(void *node)
{
    return (ft_strcmp(CMD->args[0], "echo") == 0
        || ft_strcmp(CMD->args[0], "export") == 0
        || ft_strcmp(CMD->args[0], "pwd") == 0
        || ft_strcmp(CMD->args[0], "env") == 0
        || ft_strcmp(CMD->args[0], "cd") == 0
        || ft_strcmp(CMD->args[0], "exit") == 0
    );
}
