#include "main.h"

void free_array(char **array)
{
    for (int i = 0; array[i]; i++)
        free(array[i]);
    free(array);
}

char    *cmd_path(t_db *db, char *filename)
{
    char **paths;
    char *path;
    char *tmp;

    if (!filename)
        return NULL;
    (void)db;
    paths = ft_split(getenv("PATH"), ':');
    if (!paths)
        return NULL;

    for (int i = 0; paths[i]; i++)
    {
        path = ft_strjoin(paths[i], "/");
        if (!path)
            return NULL;

        tmp = ft_strjoin(path, filename);
        if (!tmp)
            return NULL;

        free(path);
        if (access(tmp, F_OK) == 0)
        {
            free_array(paths);
            return tmp;
        }
        free(tmp);
    }

    return NULL;
}