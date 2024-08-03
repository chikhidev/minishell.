#include "exec.h"
#include "main.h"

int pipex(t_db  *db, int   n_childs)
{
    if (n_childs == 1)
    {
        char    **args;
        args = ft_split(db, "ls -la", " ");
        for (int i = 0; i < n_childs; i++)
        {
            int id = fork();
            if (id == 0)
            {
                args[0] = ft_split(db, "ls -la", " ");
                execve("/bin/ls", args, NULL);
            }
            else
                waitpid(id, NULL, 0);
        }
    }
    else if (n_childs == 2)
    {
        char    **args[2];
        for (int i = 0; i < n_childs; i++)
        {
            int id = fork();
            if (id == 0)
            {
                if (i == 0)
                {
                    args[0] = ft_split(db, "ls -la", " ");
                    execve("/bin/ls", args, NULL);
                }
                else
                {
                    args[1] = ft_split(db, "wc", " ");
                    execve("/bin/wc", args, NULL);
                }
            }
            else
                waitpid(id, NULL, 0);
        }
    }
    else if (n_childs == 3)
    {
        char    **args[3];
        for (int i = 0; i < n_childs; i++)
        {
            int id = fork();
            if (id == 0)
            {
                if (i == 0)
                {
                    args[0] = ft_split(db, "ls -la", " ");
                    execve("/bin/ls", args, NULL);
                }
                else if (i == 1)
                {
                    args[1] = ft_split(db, "wc", " ");
                    execve("/bin/wc", args, NULL);
                }
                else
                {
                    args[2] = ft_split(db, "cat", " ");
                    execve("/bin/cat", args, NULL);
                }
            }
            else
                waitpid(id, NULL, 0);
        }
    }
}

int main()
{
    pipex(2);
}