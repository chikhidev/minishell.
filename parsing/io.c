#include "main.h"
#include "parsing.h"

int create_redirection(t_db *db, int type, int fd)
{
    if (type == INPUTFILE)
    {
        if (db->input_fd != STDIN_FILENO && db->input_fd != INVALID)
        {
            close(db->input_fd);
        }
        db->input_fd = fd;
        db->curr_type = INPUTFILE;
    }
    else if (type == APPENDFILE || type == OUTPUTFILE)
    {
        if (db->output_fd != STDOUT_FILENO && db->output_fd != INVALID)
        {
            close(db->output_fd);
        }
        db->output_fd = fd;
        db->curr_type = type;
    }

    return (SUCCESS);
}

int check_ambigious(t_db *db, char *file)
{
    t_exp_list *store;

    if (file[0] == '$' && file[1] != '\0')
    {
        store = get_exp_node(db->exp_list, file + 1);
        if (!store)
        {
            printf("%s: ambiguous redirect\n", file);
            return (true);
        }
        if (store && store->val && store->val[0] == '\0')
        {
            printf("%s: ambiguous redirect\n", file);
            return (true);
        }

        if (store && contains_spaces_btwn(store->val))
        {
            printf("%s: ambiguous redirect\n", file);
            return (true);
        }
    }

    return (false);
}

int open_file(t_db *db, char *file, int type)
{
    t_quote *quotes;
    int fd;
    char *tmp;

    quotes = NULL;
    if (!file || ft_strlen(file) == 0)
        return (SUCCESS);
    fd = INVALID;

    if (check_ambigious(db, file) == true)
    {
        create_redirection(db, type, INVALID);
        return FAILURE;
    }

    track_quotes(db, &quotes, file);
    if (expand(db, &file, &quotes) == FAILURE)
        return FAILURE;

    tmp = whithout_quotes(db, file);
    if (type == APPENDFILE)
        fd = open(tmp, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (type == INPUTFILE)
        fd = open(tmp, O_RDONLY);
    else if (type == OUTPUTFILE)
        fd = open(tmp, O_WRONLY  | O_CREAT | O_TRUNC, 0644);

    if (create_redirection(db, type, fd) == FAILURE)
        return (FAILURE);

    if (fd == INVALID)
    {
        perror(tmp);
        return (FAILURE);
    }

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
    int pid;
    char *line;
    char *tmp;
    int child_status;

    tmp = whithout_quotes(db, delim);
    if (!tmp)
        return error(db, NULL, "malloc failed");
    delim = tmp;
    if (pipe(pipe_fd) == -1)
        return error(db, "pipe", NULL);

    pid = fork(); /* we fork to handle signals inside the child */
    if (pid == -1)
        return error(db, "fork", NULL);
    
    if (IS_CHILD)
    {
        heredoc_signals_handling();

        close(pipe_fd[0]);

        while (1)
        {
            line = readline("> ");
            if (!line)
            {
                dprintf(2, "Warning: here-document delimited by end-of-file (wanted `%s')\n", delim);
                close(pipe_fd[1]);
                break;
            }

            if (ft_strcmp(delim, line) == 0)
            {
                printf("found delim\n");
                close(pipe_fd[1]);
                free(line);
                break;
            }

            dprintf(2, "reached here\n");

            if (expand(db, &line, NULL) == FAILURE)
            {
                close(pipe_fd[1]);
                free(line);
                ec_void(db);
                (error(db, NULL, "Malloc failed"), exit(1));
            }

            printf("after expanding\n");

            write(pipe_fd[1], line, ft_strlen(line));
            write(pipe_fd[1], "\n", 1);
            free(line);
        }

        if (db->input_fd != STDIN_FILENO && db->input_fd != INVALID)
        {
            close(db->input_fd);
        }

        gc_void(db);
        ec_void(db);
        exit(0); /* Exit normally */
    }

    /*-------------------------------Parent process------------------------------*/
    // cancel SIGINT and SIGQUIT they sound be handled by the child

    parent_signals_handling();

    close(pipe_fd[1]);
    wait(&child_status);

    catch_feedback(db, child_status);
    if (db->last_signal != 0)
        return FAILURE;
    
    db->input_fd = pipe_fd[0];
    db->curr_type = HEREDOC;

    return SUCCESS;
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




int syntax_checker(t_db *db, char *line, int *start, t_quote *quotes)
{
    int i;

    i = *start;

    skip_open_spaces(quotes, line, &i);
    if (!line[i])
    {
        return (SUCCESS);
    }

    if (line[i] == '|')
    {
        printf("Syntax error near unexpected token `%c'\n", line[i]);
        return (FAILURE);
    }

    while (line[i])
    {
        if (((ft_strncmp(&line[i], ">>", 2) == 0
            || ft_strncmp(&line[i], "<<", 2) == 0
            || ft_strncmp(&line[i], ">", 1) == 0
            || ft_strncmp(&line[i], "<", 1) == 0))
            && !is_inside_quotes_line(line, i)
            )
        {
            if (line[i] == '<' && line[i + 1] == '<')
            {
                db->heredoc_counter++;
                if (db->heredoc_counter > 15)
                    return error(db, "heredoc", "maximum here-document count exceeded");
                i++;
            }

            i += 1 + (line[i + 1] == '>' || line[i + 1] == '<');
            skip_spaces(line, &i);
            if (!line[i])
            {
                return error(db, NULL, "Syntax error near unexpected token `newline'");
            }

            if (line[i] == '|')
            {
                printf("Syntax error near unexpected token `%c\n", line[i]);
                return error(db, NULL, NULL);
            }
            if (is_op_redir(line, i) == FAILURE)
                return (FAILURE);
        }

        if (line[i] == '|')
        {
            i++;
            skip_spaces(line, &i);
            if (!line[i])
            {
                printf("Syntax error near unexpected token `newline'\n");
                return (FAILURE);
            }
            if (line[i] == '|')
            {
                printf("Syntax error near unexpected token `%c'\n", line[i]);
                return (FAILURE);
            }
        }

        i++;
    }
    return (SUCCESS);
}
