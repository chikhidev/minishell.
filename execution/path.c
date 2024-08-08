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
    t_exp_list  *path_node;

    if (!filename)
        return NULL;
    path_node = get_exp_node(db->exp_list, "PATH");
    if (!path_node)
    {
        return (NULL);
    }
    paths = ft_split(db, path_node->val, ':');
    if (!paths)
        return NULL;
    for (int i = 0; paths[i]; i++)
    {
        path = ft_strjoin(db, paths[i], "/");
        if (!path)
        {
            error(db, NULL, "Malloc failed");
            return NULL;
        }
        tmp = ft_strjoin(db, path, filename);
        if (!tmp)
        {
            error(db, NULL, "Malloc failed");
            return NULL;
        }

        if (access(tmp, F_OK) == 0)
        {
            free_array(db, paths);
            if (access(tmp, X_OK) != 0)
            {
                error(db, tmp, "Permission denied");
                return NULL;
            }
            return tmp;
        }
    }

    return NULL;
}
