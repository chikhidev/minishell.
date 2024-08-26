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
        || ft_strcmp(CMD->args[0], "unset") == 0
    );
}

int run_builtin(t_db   *db,t_cmd_node *node, int index)
{
    if (ft_strcmp(CMD->args[0], "echo") == 0)
        return echo_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "export") == 0)
        return export_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "pwd") == 0)
        return pwd_(db);
    else if (ft_strcmp(CMD->args[0], "env") == 0)
        return env_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "cd") == 0)
        return cd_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "unset") == 0)
        return unset_(db, CMD->args);
    else if (ft_strcmp(CMD->args[0], "exit") == 0)
        return exit_(db,   CMD->args, index);
    return 127;
}
