#include "includes/main.h"
#include "includes/parsing.h"

void    ascii_print()
{
    printf("\n\n");
    printf(ORANGE"█░░ █▀█ █░█░█ ▄▀█ █▀ █░█ █▀▀ █░░ █░░"RESET"\n");
    printf(GREEN"█░▄ █▀▄ ▀▄▀▄▀ █▀█ ▄█ █▀█ █▄▄ █▄▄ █▄▄"RESET"\n");
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
    tmp = ft_strjoin(GREEN"Lowa@"RESET, user);
    free(user);
    if (!tmp)
        return (ft_strdup(GREEN"Lowa@shell~"RESET));
    user = ft_strjoin(tmp, ORANGE"~ "RESET);
    free(tmp);
    if (!user)
        return (ft_strdup(GREEN"Lowa@shell~"RESET));
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
    int i;

    db->gc = NULL;
    db->root_node = NULL;
    db->quotes = NULL;
    db->paranthesis = NULL;
    i = 0;
    while (i < 6)
    {
        db->op_counter[i] = 0;
        i++;
    }
}

int     main()
{
    t_db    db;
    char    *line;
    int     ret;

    line = NULL;
    ascii_print();
    while (TRUE)
    {
        init_db(&db);
        ret = handle_prompt(&line);
        if (ret == -1) break ;
        else if (ret == 0) continue ;
        /* parse the line ----------------------------------*/
        if (parser(&db, line) == FAILURE)
        {
            free(line);
            continue ;
        }
        /* execute the cmds --------------------------------*/

        gc_void(&db);
        free(line);
    }
    free(line);
    gc_void(&db);
    return (SUCCESS);
}
