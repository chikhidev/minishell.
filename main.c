#include "includes/main.h"
#include "includes/parsing.h"

void    ascii_print()
{
    printf("\n\n");
    printf(ORANGE"█░░ █▀█ █░█░█ ▄▀█ █▀ █░█ █▀▀ █░░ █░░"RESET"\n");
    printf(MAGENTA"█░▄ █▀▄ ▀▄▀▄▀ █▀█ ▄█ █▀█ █▄▄ █▄▄ █▄▄"RESET"\n");
    printf("\n");
    printf("Welcome to shell of abdoo and salah 🚀\n");
    printf("--------------------------------------------\n\n");
}

char    *line_promte()
{
    char    *user;
    char    *tmp;

    user = ft_strdup(getenv("USERNAME"));
    if (!user)
        user = ft_strdup("shell");
    tmp = ft_strjoin(MAGENTA"Lowa@"RESET, user);
    free(user);
    if (!tmp)
        return (ft_strdup(MAGENTA"Lowa@shell~"RESET));
    user = ft_strjoin(tmp, ORANGE"~ "RESET);
    free(tmp);
    if (!user)
        return (ft_strdup(MAGENTA"Lowa@shell~"RESET));
    return (user);
}

int handle_prompt(char **line)
{
    char    *promte;

    promte = line_promte();
    *line = readline(promte);
    free(promte);
    // handle ctrl + c later 
    if (!*line) return 0 ; // continue the loop
    if (ft_strncmp(*line, "exit", 4) == 0) return -1 ; // break the loop
    if (*line[0] != '\0') add_history(*line);
    return 1 ; // nothing
}

void    init_db(t_db *db)
{
    db->gc = NULL;
}

int     main()
{
    t_db    db;
    char    *line;
    int     ret;

    line = NULL;
    init_db(&db);
    ascii_print();
    while (TRUE)
    {
        ret = handle_prompt(&line);
        if (ret == -1) break ;
        else if (ret == 0) continue ;
        /* parse the line ----------------------------------*/
        if (parser(&db, line) == FAILURE)
        {
            free(line);
            gc_free(&db);
            continue ;
        }
        /* execute the cmds --------------------------------*/

        free(line);
    }
    free(line);
    gc_free(&db);
    return (SUCCESS);
}