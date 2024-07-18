#include "main.h"
#include "parsing.h"

int create_redirection(t_db *db, t_redirection **redirections, int type, int fd)
{
    t_redirection *new;
    t_redirection *tmp;


    new = gc_malloc(db, sizeof(t_redirection));
    CATCH_MALLOC(new);
    new->type = type;
    new->fd = fd;
    new->next = NULL;

    if (!*redirections)
    {
        *redirections = new;
        return (SUCCESS);
    }

    tmp = *redirections;
    while (tmp->next)
    {
        tmp = tmp->next;
    }

    tmp->next = new;

    return (SUCCESS);
}


int io_system(t_db *db, char **args, t_redirection **res)
{
    t_iterators it;
    int fd;

    fd = INVALID;
    it.i = 0; // to iterate over the args

    while (args[it.i])
    {
        if (ft_strncmp(args[it.i], ">", 1) == 0 || ft_strncmp(args[it.i], ">>", 2) == 0)
        {
            if (!args[it.i + 1])
            {
                printf("Out of args\n");
                return (FAILURE);
            }

            if (ft_strncmp(args[it.i], ">", 1) == 0)
                fd = open(args[it.i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (ft_strncmp(args[it.i], ">>", 2) == 0)
                fd = open(args[it.i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);

            if (fd == INVALID)
            {
                perror(args[it.i + 1]);
                error(db, NULL, NULL);
                return (FAILURE);
            }
            CATCH_ONFAILURE(
                create_redirection(db, res, APPENDFILE - ft_strncmp(args[it.i], ">>", 2), fd),
                FAILURE
            )
            it.i++;
        }
        else if (ft_strncmp(args[it.i], "<", 1) == 0)
        {
            if (!args[it.i + 1])
            {
                printf("Out of args\n");
                return (FAILURE);
            }

            fd = open(args[it.i + 1], O_RDONLY);
            if (fd == INVALID)
            {
                perror(args[it.i + 1]);
                error(db, NULL, NULL);
                return (FAILURE);
            }
            CATCH_ONFAILURE(
                create_redirection(db, res, INPUTFILE, fd),
                FAILURE
            )
            it.i++;
        }
        else
        {
            // check if it's at the same same arg: ">file.txt" or "<file.txt"
            PASS;
        }
        it.i++;
    }

    return (SUCCESS);
}

