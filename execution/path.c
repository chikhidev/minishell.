#include "main.h"

void free_array(t_db *db, char **array)
{
    for (int i = 0; array[i]; i++)
        gc_free(db, array[i]);
    gc_free(db, array);
}

char    *cmd_path(t_db *db, char *filename)
{
    char **paths;
    char *path;
    char *tmp;

    if (!filename)
        return NULL;
    (void)db;
    paths = ft_split(db, getenv("PATH"), ':');
    if (!paths)
        return NULL;

    for (int i = 0; paths[i]; i++)
    {
        path = ft_strjoin(db, paths[i], "/");
        if (!path)
            return NULL;

        tmp = ft_strjoin(db, path, filename);
        if (!tmp)
            return NULL;

        gc_free(db, path);
        if (access(tmp, F_OK) == 0)
        {
            free_array(db, paths);
            return tmp;
        }
        gc_free(db, tmp);
    }

    return NULL;
}