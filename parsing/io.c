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
    if (ft_strncmp(arg, ">", 1) == 0 && size == 1)
        return (OUTPUTFILE);
    if (ft_strncmp(arg, ">>", 2) == 0 && size == 2)
        return (APPENDFILE);
    if (ft_strncmp(arg, "<", 1) == 0 && size == 1)
        return (INPUTFILE);
    return (INVALID);
}

// int io_system(t_db *db, char **args, t_redirection **res)
// {
//     t_iterators it;
//     int fd;

//     fd = INVALID;
//     it.i = 0; // to iterate over the args

//     while (args[it.i])
//     {
//         if (ft_strncmp(args[it.i], ">", 1) == 0 || ft_strncmp(args[it.i], ">>", 2) == 0)
//         {
//             if (!args[it.i + 1])
//             {
//                 printf("Out of args\n");
//                 return (FAILURE);
//             }

//             if (ft_strncmp(args[it.i], ">", 1) == 0)
//                 fd = open(args[it.i + 1], O_WRONLY | O_CREAT | O_TRUNC, 0644);
//             if (ft_strncmp(args[it.i], ">>", 2) == 0)
//                 fd = open(args[it.i + 1], O_WRONLY | O_CREAT | O_APPEND, 0644);

//             if (fd == INVALID)
//             {
//                 perror(args[it.i + 1]);
//                 error(db, NULL, NULL);
//                 return (FAILURE);
//             }
//             CATCH_ONFAILURE(
//                 create_redirection(db, res, APPENDFILE - ft_strncmp(args[it.i], ">>", 2), fd),
//                 FAILURE
//             )
//             it.i++;
//         }
//         else if (ft_strncmp(args[it.i], "<", 1) == 0)
//         {
//             if (!args[it.i + 1])
//             {
//                 printf("Out of args\n");
//                 return (FAILURE);
//             }

//             fd = open(args[it.i + 1], O_RDONLY);
//             if (fd == INVALID)
//             {
//                 perror(args[it.i + 1]);
//                 error(db, NULL, NULL);
//                 return (FAILURE);
//             }
//             CATCH_ONFAILURE(
//                 create_redirection(db, res, INPUTFILE, fd),
//                 FAILURE
//             )
//             it.i++;
//         }
//         else
//         {
//             // check if it's at the same same arg: ">file.txt" or "<file.txt"
//             PASS;
//         }
//         it.i++;
//     }

//     return (SUCCESS);
// }

