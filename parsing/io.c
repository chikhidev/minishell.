#include "main.h"
#include "parsing.h"
# include "../get_next_line/get_next_line.h"

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
            return (TRUE);
        }
        if (store && store->val && store->val[0] == '\0')
        {
            printf("%s: ambiguous redirect\n", file);
            return (TRUE);
        }

        if (store && contains_spaces_btwn(store->val))
        {
            printf("%s: ambiguous redirect\n", file);
            return (TRUE);
        }
    }

    return (FALSE);
}

int open_file(t_db *db, char *file, int type, t_quote **quotes)
{
    int fd;
    char *tmp;

    if (!file || ft_strlen(file) == 0)
        return (SUCCESS);
    fd = INVALID;
    if (check_ambigious(db, file) == TRUE)
    {
        create_redirection(db, type, INVALID);
        return FAILURE;
    }
    expand(db, &file, quotes);
    tmp = whithout_quotes(db, file);
    if (type == APPENDFILE)
        fd = open(tmp, O_WRONLY | O_CREAT | O_APPEND, 0644);
    else if (type == INPUTFILE)
        fd = open(tmp, O_RDONLY);
    else if (type == OUTPUTFILE)
        fd = open(tmp, O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fd == INVALID)
    {
        perror(tmp);
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

    tmp = whithout_quotes(db, delim);
    if (!tmp)
        return error(db, NULL, "malloc failed");
    delim = tmp;
    if (pipe(pipe_fd) == -1)
        return error(db, "pipe", NULL);
    write(2, "> ", 2);
    while (1)
    {
        line = get_next_line(db, 0);
        if (!line)
        {
            write(2, "\n", 1);
            close(pipe_fd[1]);
            break;
        }
        tmp = gc_copy(db, line);
        gc_free(db, line);
        if (!tmp)
            return error(db, NULL, "malloc failed");
        line = tmp;
        if (is_newline_at_the_end(line))
        {
            tmp = ft_substr(db,     line, 0, ft_strlen(line) - 1);
            if (!tmp)
                return error(db, NULL, "malloc failed");
            line = gc_copy(db, tmp);
            gc_free(db, tmp);
        }
        if (ft_strcmp(delim, line) == 0
            && ft_strlen(delim) == ft_strlen(line))
        {
            write(2, "\n", 1);
            close(pipe_fd[1]);
            break;
        }
        CATCH_ONFAILURE(
            expand(db, &line, NULL),
            FAILURE
        )
        write(pipe_fd[1], line, ft_strlen(line));
        write(2, "> ", 2);
    }
    if (db->input_fd != STDIN_FILENO && db->input_fd != INVALID)
    {
        close(db->input_fd);
    }
    db->input_fd = pipe_fd[0];
    gc_free(db, delim);
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
            if (line[i] == '<' && line[i + 1] == '<')
            {
                db->heredoc_counter++;
                if (db->heredoc_counter > 16)
                    return error(db, "heredoc", "maximum here-document count exceeded");
                i++;
            }
            
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

        i++;
    }
    return (SUCCESS);
}

