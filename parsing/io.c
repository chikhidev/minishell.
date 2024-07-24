#include "main.h"
#include "parsing.h"

int create_redirection(t_db *db, int type, int fd)
{
    if (type == INPUTFILE)
    {
        db->input_fd = fd;
        db->curr_type = INPUTFILE;
    }
    else if (type == OUTPUTFILE)
    {
        db->output_fd = fd;
        db->curr_type = OUTPUTFILE;
    }
    else if (type == APPENDFILE)
    {
        db->output_fd = fd;
        db->curr_type = APPENDFILE;
    }

    return (SUCCESS);
}

int open_file(t_db *db, char *file, int type, t_quote *quotes)
{
    int fd;
    
    expand(db, &file, quotes);

    if (type == APPENDFILE)
        fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (type == INPUTFILE)
        fd = open(file, O_RDONLY);
    else if (type == OUTPUTFILE)
        fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == INVALID)
    {
        perror(file);
        db->error = TRUE;
        return FAILURE;
    }

    if (create_redirection(db, type, fd) == FAILURE)
        return (FAILURE);

    return (SUCCESS);
}

int validate_io(char *arg, int size)
{
    if (ft_strncmp(arg, ">>", 2) == 0 && size == 2)
        return (APPENDFILE);
    if (ft_strncmp(arg, "<<", 2) == 0 && size == 2)
        return (HEREDOC);
    if (ft_strncmp(arg, ">", 1) == 0 && size == 1)
        return (OUTPUTFILE);
    if (ft_strncmp(arg, "<", 1) == 0 && size == 1)
        return (INPUTFILE);
    return (INVALID);
}

int open_heredoc(t_db *db, char *delim)
{
    int pipe_fd[2];
    char *line;

    if (pipe(pipe_fd) == -1)
    {
        error(db, "pipe", NULL);
        return (FAILURE);
    }
    write(2, "> ", 2);
    while (1)
    {
        line = get_next_line(0);
        if (line == NULL || 
            (ft_strncmp(line, delim, ft_strlen(delim)) == 0
            && ft_strlen(line) - 1 == ft_strlen(delim)))
        {
            close(pipe_fd[1]);
            break;
        }
        write(pipe_fd[1], line, ft_strlen(line));
        free(line);
        write(2, "> ", 2);
    }
    db->input_fd = pipe_fd[0];
    return (SUCCESS);
}
