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
    
    fd = INVALID;
    expand(db, &file, quotes);
    printf("file -> %s type => %d", file , type);
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
    char *tmp;

    if (pipe(pipe_fd) == -1)
        return error(db, "pipe", NULL);
    write(2, "(heredoc)> ", 12);
    while (1)
    {
        line = get_next_line(0);
        if (is_newline_at_the_end(line))
        {
            tmp = ft_substr(line, 0, ft_strlen(line) - 1);
            free(line);
            line = tmp;
        }
        if (line == NULL
            || (
                ft_strcmp(delim, line) == 0
                && ft_strlen(delim) == ft_strlen(line)
            )
        )
        {
            write(2, "\n", 1);
            free(line);
            close(pipe_fd[1]);
            break;
        }
        expand(db, &line, NULL);
        write(pipe_fd[1], line, ft_strlen(line));
        free(line);
        write(2, "(heredoc)> ", 12);
    }
    db->input_fd = pipe_fd[0];
    return (SUCCESS);
}

int is_op_redir(char *line, int i)
{
    if (ft_strncmp(&line[i], ">>", 2) == 0
        || ft_strncmp(&line[i], "<<", 2) == 0)
    {
        printf("Syntax error near unexpected token `%c%c'\n", line[i], line[i + 1]);
        return (FAILURE);
    }

    if (ft_strncmp(&line[i], ">", 1) == 0
        || ft_strncmp(&line[i], "<", 1) == 0)
    {
        printf("Syntax error near unexpected token `%c'\n", line[i]);
        return (FAILURE);
    }

    return (SUCCESS);
}

int syntax_checker(t_db *db, char *line, int *start)
{
    int i;

    i = *start;
    while (line[i])
    {

        if (ft_strncmp(&line[i], ">>", 2) == 0
            || ft_strncmp(&line[i], "<<", 2) == 0
            || ft_strncmp(&line[i], ">", 1) == 0
            || ft_strncmp(&line[i], "<", 1) == 0)
        {
            i += 1 + (line[i + 1] == '>' || line[i + 1] == '<');
            skip_spaces(line, &i);
            if (!line[i])
            {
                return error(db, NULL, "Syntax error near unexpected token `newline'");
            }

            if (line[i] == ')' || line[i] == '(' || line[i] == '|' || line[i] == '&')
            {
                printf("Syntax error near unexpected token `%c", line[i]);
                if (line[i + 1] == '|' || line[i + 1] == '&')
                    printf("%c", line[i + 1]);
                printf("'\n");
                return error(db, NULL, NULL);
            }
            if (is_op_redir(line, i) == FAILURE)
                return (FAILURE);
        }


        if (line[i] == '<' && line[i + 1] == '<')
        {
            skip_spaces(line, &i);

            db->heredoc_counter++;
            if (db->heredoc_counter > 16)
                return error(db, "heredoc", "maximum here-document count exceeded");
            i++;
        }
        i++;
    }
    return (SUCCESS);
}


int handle_heredocs(t_db *db, char *line)
{
    int i;
    int start;
    int ret;
    char *tmp;

    i = 0;
    start = 0;
    while (line[i])
    {
        if (line[i] == '<' && line[i + 1] == '<')
        {
            start = i;
            i += 2;
            while (line[i] && line[i] != '\n'
                && validate_io(&line[i], 1) == INVALID)
                i++;
            tmp = ft_substr(line, start + 2, i - start - 2);
            ret = open_heredoc(db, tmp);
            free(tmp);
            if (ret == FAILURE)
                return (FAILURE);
            if (i >= (int)ft_strlen(line))
                break;
        }
        i++;
    }
    return (SUCCESS);
}