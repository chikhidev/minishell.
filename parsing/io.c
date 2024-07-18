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

int io_system(t_db *db, char *line, t_redirection **res)
{
    t_iterators it;
    char flag; // 0 => input, 1 => output, 2 => append
    char *filename;
    int fd;

    fd = INVALID;
    it.i = 0;
    while (line[it.i])
    {
        if (line[it.i] == '>' || line[it.i] == '<')
        {
            if (line[it.i] == '>')
            {
                flag = OUTPUTFILE;
                if (line[it.i + 1] && line[it.i + 1] == '>')
                {
                    flag = APPENDFILE;
                    it.i++;
                }
            }
            else if (line[it.i] == '<')
                flag = INPUTFILE;

            it.i++;

            skip_spaces(line, &it.i);
            if (!line[it.i])
            {
                printf("Out of string\n");
                return (FAILURE);
            }
            
            it.j = it.i;
            while (line[it.j] && !is_whitespace(line[it.j]))
                it.j++;
            
            printf("i: %d, j: %d\n", it.i, it.j);
            filename = sub(db, line, it.i, it.j);
            CATCH_ONNULL(filename, error(db, NULL, "Malloc failed"));
            printf("filename: %s\n", filename);

            if (flag == INPUTFILE && ft_strncmp(filename, "/dev/stdin", ft_strlen(filename)) == 0)
            {
                fd = STDIN_FILENO;
            }
            else if ((flag == OUTPUTFILE || flag == APPENDFILE)
                && ft_strncmp(filename, "/dev/stdout", ft_strlen(filename)) == 0)
            {
                fd = STDOUT_FILENO;
            }
            else
            {
                if (flag == INPUTFILE)
                    fd = open(filename, O_RDONLY);
                else if (flag == OUTPUTFILE)
                    fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                else if (flag == APPENDFILE)
                    fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
            }


            it.i = it.j;
            
            if (fd == INVALID)
            {
                perror(filename);
                error(db, NULL, NULL);
                return (FAILURE);
            }

            CATCH_ONFAILURE(
                create_redirection(db, res, flag, fd),
                FAILURE
            )

            it.i--;
        }
        it.i++;
    }

    return (SUCCESS);
}