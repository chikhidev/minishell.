#include "main.h"
#include "parsing.h"
#include "exec.h"
#include "builtens.h"

char    *line_promte()
{
    char    *user;
    char    *tmp;

    user = getenv("USER");
    if (user)
        user = ft_strdup(user);
    else
        user = ft_strdup("shell");
    if (!user)
        return (NULL);
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

void set_env_lst(t_db   *db, char   *env)
{
    
}

void    init_db(t_db *db, int ac, char *av[], char *env[])
{
    int i;

    db->debug = FALSE;
    db->gc = NULL;
    db->root_node = NULL;
    db->ops = NULL;
    db->here_docs = NULL;
    db->error = FALSE;
    db->env = env;
    db->last_signal = 0;
    db->redirections = NULL;
    (void) ac;
    (void) av;
    i = 0;
    while (i < 6)
    {
        db->op_counter[i] = 0;
        i++;
    }
}

int     main(int    ac, char    *av[],  char    *env[])
{
    t_db    db;
    char    *line;
    char    *tmp;
    int     ret;

    line = NULL;
    while (TRUE)
    {
        init_db(&db, ac, av, env);
        printf(MAGENTA);
        ret = handle_prompt(&line);
        printf(RESET);
        if (ret == -1) break ;
        else if (ret == 0) continue ;

        tmp = gc_malloc(&db, ft_strlen(line) + 1);
        if (!tmp) return !error(&db, NULL, "malloc failed");
        ft_strlcpy(tmp, line, ft_strlen(line) + 1);
        free(line);
        line = tmp;

        /* parse the line ----------------------------------*/
        if (parser(&db, line) == FAILURE)
            continue ;
        if (exec(&db, db.root_node) == FAILURE)
            continue ;
        /* execute the cmds --------------------------------*/
        gc_void(&db);
    }
    gc_void(&db);
    return (SUCCESS);
}
