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

t_env_list    *set_env_lst(t_db   *db, char   *env[])
{
    t_env_list      *env_list;
    t_env_list      *new_node;
    int             i;

    env_list = NULL;
    i = 0;
    while (env[i])
    {
        new_node = new_env_node(db, env[i]);
        if (!new_node)
            (gc_void(db), exit(1));
        push_env_back(&env_list, new_node);
        i++;
    }
    return env_list;
}

t_exp_list    *set_exp_lst(t_db   *db, char   *env[])
{
    t_exp_list      *exp_list;
    t_exp_list      *new_node;
    int             i;
    int             len;
    char            *key;
    char            *val;

    exp_list = NULL;
    i = 0;
    while (env[i])
    {
        new_node = new_exp_node(db, env[i], NULL);
        if (!new_node)
            (gc_void(db), exit(1));
        len = length_til(env[i], '=');
        key = gc_malloc(db, len + 1);
        ft_strlcpy(key, env[i], len + 1);
        val = gc_malloc(db, ft_strlen(env[i]) - len + 1);
        ft_strlcpy(val, env[i] + len, ft_strlen(env[i]) - len + 1);
        new_node->key = key;
        new_node->val = val;
        new_node->next = NULL;
        if (ft_strncmp(key, "_", ft_strlen(key)) != 0)
            push_exp_back(&exp_list, new_node);
        i++;
    }
    return exp_list;
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
    db->curr_type = INVALID;
    db->input_fd = INVALID;
    db->output_fd = INVALID;
    db->env_list = set_env_lst(db, env);
    db->exp_list = set_exp_lst(db, env);
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
    // int i = 0;
    // while (env[i])
    //     printf("%s\n", env[i++]);
    t_db    db;
    // t_env_list  *env_list;
    char    *line;
    char    *tmp;
    int     ret;

    line = NULL;
    init_db(&db, ac, av, env);
    while (TRUE)
    {
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
        // env_list = db.env_list;
        // while (env_list)
        // {
        //     printf("%s\n", env_list->data);
        //     env_list = env_list->next;
        // }
        gc_void(&db);
    }
    gc_void(&db);
    return (SUCCESS);
}
